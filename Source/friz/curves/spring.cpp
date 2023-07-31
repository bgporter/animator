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
#include "spring.h"

namespace friz
{
Spring::Spring (float startVal, float endVal, float tolerance, float accel,
                float damping_)
: ToleranceValue (startVal, endVal, tolerance)
, acceleration (accel)
, damping (damping_)
, velocity (0)
{
    // the damping factor must be between 0..1.
    jassert (juce::isPositiveAndBelow (damping, 1.f));
    if (endVal < startVal)
        acceleration *= -1;
}

float Spring::generateNextValue ()
{
    // a weird case we need to handle -- if we're already at our end state,
    // remain there! Dont' keep oscillating.
    if (isFinished ())
        return endVal;

    float prev = currentVal;
    velocity += acceleration;
    float next = prev + velocity;

    // see if we have crossed over the end value
    // if so, we may need to change direction and dampen the oscillation
    if ((endVal - prev) * (endVal - next) <= 0)
    {
        if (std::fabs (damping) < 0.001)
            // we overshot the end and don't want to oscillate. Clamp to the
            // end value instead.
            next = endVal;
        else
        {
            // reverse our acceleration direction and reduce it by the
            // damping factor; we'll keep oscillating around the end value
            // until we're within the tolerance value of it, at which
            // point we'll stop.
            acceleration = -1 * acceleration * damping;
            // velocity = 0.f;
            velocity *= -damping;
        }
    }

    return next;
}

#ifdef qRunUnitTests
#include "test/test_Spring.cpp"
#endif

} // namespace friz
