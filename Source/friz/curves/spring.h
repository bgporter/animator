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

#include "animatedValue.h"

namespace friz
{

/**
 * @class Spring
 * @brief Accelerates toward the end value -- if this acceleration takes it past
 *        the end value, it enters a mode where it acts like a simplified
 *        oscillating spring; the behavior of the oscillation can be loosened or
 *        tightened using the `dampen` parameter.
 */

class Spring : public ToleranceValue
{
public:
    /**
     * A curve that accelerates toward the end value, then oscillates toward it
     * on overshoot.
     * @param startVal  initial value
     * @param endVal    end value.
     * @param tolerance How close do we need to be to stop animating?
     * @param accel     Velocity change on each update.
     * @param dampen    How much to dampen the oscillation, 0 < dampen < 1.
     *                  smaller values dampen the oscillation faster.
     */
    Spring (float startVal, float endVal, float tolerance, float accel, float dampen);

private:
    float generateNextValue () override;

private:
    /// When we're in a damping state, the acceleration will change.
    float acceleration;

    /// damping factor to apply when we need to oscillate toward end val.
    float damping;

    /// current velocity, updated with acceleration every frame.
    float velocity;
};

} // namespace friz
