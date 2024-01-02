/*
    Copyright (c) 2019-2023 Brett g Porter

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "controller.h"

#include "animator.h"

namespace friz
{
juce::int64 Controller::getCurrentTime ()
{
    auto nowInTix { juce::Time::getHighResolutionTicks () };
    auto nowInMs { 1000 * juce::Time::highResolutionTicksToSeconds (nowInTix) };
    return static_cast<juce::int64> (nowInMs + 0.5);
}

void FrameRateCalculator::update (juce::int64 timeInMs)
{
    if (lastUpdate > 0)
    {
        int currentSum { sum.load () };
        currentSum -= memory[index];
        const auto delta { static_cast<int> (timeInMs - lastUpdate) };
        currentSum += delta;
        memory[index++] = delta;
        index %= frameCount;
        sum.store (currentSum);
        updateCount++;
    }

    lastUpdate = timeInMs;
}

float FrameRateCalculator::get () const
{
    // convert the average interval between updates into a rate/sec
    const int divisor = std::min (updateCount.load (), frameCount);
    if (divisor > 0)
        return 1000.f / (static_cast<float> (sum.load ()) / static_cast<float> (divisor));
    return 0.f;
}

void FrameRateCalculator::clear ()
{
    lastUpdate = -1;
    sum.store (0);
    memory.fill (0);
    index = 0;
    updateCount.store (0);
}

void TimeController::timerCallback ()
{
    animator->gotoTime (getCurrentTime ());
}

#if FRIZ_VBLANK_ENABLED
void DisplaySyncController::start ()
{
    jassert (animator != nullptr);
    if (sync.isEmpty () && animator != nullptr)
    {
        frameRate.clear ();
        sync = { syncSource, [this]
                 {
                     const auto now { getCurrentTime () };
                     animator->gotoTime (now);
                     frameRate.update (now);
                 } };
        running = true;
    }
}
#endif

bool AsyncController::gotoTime (juce::int64 timeInMs)
{
    if (!isRunning ())
        return false;

    if (timeInMs <= lastTime)
    {
        // time can only go forward!
        jassertfalse;
        return false;
    }
    animator->gotoTime (timeInMs);
    frameRate.update (timeInMs);
    lastTime = timeInMs;
    return true;
}

} // namespace friz
