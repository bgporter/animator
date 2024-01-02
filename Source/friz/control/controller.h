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

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#if JUCE_VERSION >= (7 << 16)
#define FRIZ_VBLANK_ENABLED 1
#else
#define FRIZ_VBLANK_ENABLED 0
#endif
namespace friz
{
class Animator;
/**
 * @class FrameRateCalculator
 * @brief Calculate the actual current (average) frame rate as measured
 * at runtime.
 *
 */
class FrameRateCalculator
{
public:
    FrameRateCalculator () = default;

    /**
     * @brief Called each time we update the animator so we can keep track
     * of the frequency.
     *
     * @param timeInMs
     */
    void update (juce::int64 timeInMs);

    /**
     * @brief Calculate the actual frame rate that we're running at.
     *
     * @return float frames per second (averaged over recent history)
     */
    float get () const;

    /**
     * @brief reset all internal values before starting.
     */
    void clear ();

private:
    juce::int64 lastUpdate { -1 };
    static constexpr int frameCount { 24 };
    std::array<int, frameCount> memory;
    /// @brief  keep a running sum of intervals so we can just divide.
    std::atomic<int> sum { 0 };
    std::atomic<int> updateCount { 0 };
    size_t index { 0 };
};

class Controller
{
public:
    Controller ()                             = default;
    virtual ~Controller ()                    = default;
    Controller (const Controller&)            = delete;
    Controller& operator= (const Controller&) = delete;
    Controller (Controller&&)                 = delete;
    Controller& operator= (Controller&&)      = delete;

    /** Point back at the Animator object that we're going to control.
     */
    void setAnimator (Animator* animator_) { animator = animator_; }

    /**
     * Set our frame rate to match that used in the animation we're controlling.
     * @param frameRate
     */
    virtual bool setFrameRate (int /*frameRate*/) { return false; }

    /**
     * @brief report the current frame rate, whether what was requested or the actual
     * measured rate.
     *
     * @return float
     */
    virtual float getFrameRate () const { return 0.f; }

    /**
     * @brief Called whenever we need to start timer callbacks flowing.
     */
    virtual void start () = 0;

    /**
     * @brief  Called whenever there are no more animations that need to
     * be updated.
     */
    virtual void stop () = 0;

    /**
     * @brief Test to see if the timer is currently running.
     *
     */
    virtual bool isRunning () const = 0;

    /**
     * @brief Calculate the current time in milliseconds since some event,
     * probably system start. Probably not accurate enough for e.g. musical purposes
     * but more than accurate enough for visual animations.
     *
     * @return int64 millisecond value.
     */
    static juce::int64 getCurrentTime ();

protected:
    /// @brief  the animator object that owns us.
    Animator* animator;
};

class TimeController : public Controller,
                       public juce::Timer
{
public:
    bool setFrameRate (int frameRate_) override
    {
        jassert (frameRate_ > 0);
        frameRate = frameRate_;
        return true;
    }

    /**
     * @brief report the current frame rate, whether what was requested or the actual
     * measured rate.
     *
     * @return float
     */
    float getFrameRate () const override { return isRunning () ? static_cast<float> (frameRate) : 0.f; }

    /**
     * @brief Called whenever we need to start timer callbacks flowing.
     */
    void start () override
    {
        jassert (animator != nullptr);
        startTimerHz (frameRate);
    }

    /**
     * @brief  Called whenever there are no more animations that need to
     * be updated.
     */
    void stop () override { stopTimer (); }

    /**
     * @brief Test to see if the timer is currently running.
     *
     */
    bool isRunning () const override { return isTimerRunning (); }

private:
    void timerCallback () override;

private:
    /// @brief Approx. frames/sec
    int frameRate { 30 };
};

#if FRIZ_VBLANK_ENABLED
/**
 * @class DisplaySyncController
 *
 * @brief Synchronize animations with a display's vertical blank interval.
 *
 * @warning This class is only available in code targeting JUCE 7.0.0 or higher
 *
 */
class DisplaySyncController : public Controller
{
public:
    DisplaySyncController (juce::Component* syncSource_)
    : syncSource { syncSource_ }
    {
    }

    virtual float getFrameRate () const override
    {
        return isRunning () ? frameRate.get () : 0.f;
    }

    /**
     * @brief Called whenever we need to start timer callbacks flowing.
     */
    virtual void start () override;

    /**
     * @brief  Called whenever there are no more animations that need to
     * be updated.
     */
    virtual void stop () override
    {
        running = false;
        sync    = {};
    }

    /**
     * @brief Test to see if the timer is currently running.
     *
     */
    virtual bool isRunning () const override
    {
        // if/when JUCE makes this isEmpty() call const-qualified,
        // we can remove the separate boolean to track whether we're
        // running or not.
        // return !sync.isEmpty ();
        return running;
    }

private:
    /// @brief  We'll be updated (via our callback lambda) on each vertical blank
    /// interval of the display that is showing this component.
    juce::Component* syncSource;

    /// @brief The VBlank listener that will update this controller.
    juce::VBlankAttachment sync;

    FrameRateCalculator frameRate;
    bool running { false };
};
#endif
/**
 * @class AsyncController
 * @brief Controller to support clocking an animation manually, or at rates
 * that aren't tied to wall-clock time (e.g. rendering an animation faster than
 * realtime)
 */
class AsyncController : public Controller
{
public:
    AsyncController () = default;

    virtual float getFrameRate () const override { return frameRate.get (); }

    void start () override { running = true; }

    void stop () override { running = false; }

    bool isRunning () const override { return running; }

    /**
     * @brief Manually advance the animation to a point in time. Each call
     * to this method must move time forward!
     *
     * @param timeInMs
     * @return false
     */
    bool gotoTime (juce::int64 timeInMs);

private:
    FrameRateCalculator frameRate;
    bool running { false };
    juce::int64 lastTime { 0 };
};

} // namespace friz
