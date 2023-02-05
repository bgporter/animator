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
class EasingCurve : public ToleranceValue
{
public:
    EasingCurve (float startVal, float endVal, float tolerance, float slewRate_);

protected:
    float slewRate;
};

/**
 * @class EaseIn
 *
 * @brief A slew-based ease in curve -- accelerates quickly, then decelerates
 *        as it approaches the end value.
 *
 */

class EaseIn : public EasingCurve
{
public:
    /**
     * Decelerate into the end value.
     * @param startVal  start value
     * @param endVal    end value
     * @param tolerance Tolerance for stopping.
     * @param slewRate  slew rate, must be 0 < rate < 1
     */
    EaseIn (float startVal, float endVal, float tolerance, float slewRate);

private:
    float generateNextValue () override;
};

/**
 * @brief An animated value whose end value can be changed while the animation
 *        is in progress.
 */
class SmoothedValue : public EaseIn
{
public:
    SmoothedValue (float startVal, float endVal, float tolerance, float slewRate)
    : EaseIn (startVal, endVal, tolerance, slewRate)
    {
    }

    /**
     * @brief Update the target value while the animation is running.
     *
     * @param newTarget
     */
    bool updateTarget (float newTarget) override
    {
        endVal = newTarget;
        return true;
    }
};

/**
 * @class EaseOut
 *
 * @brief A slew-based acceleration. starts slowly & accelerates.
 */
class EaseOut : public EasingCurve
{
public:
    /**
     * Accelerate into the end value.
     * @param startVal  start val
     * @param endVal    end val
     * @param tolerance tolerance for stopping
     * @param slewRate  slew rate, must be > 1.
     */
    EaseOut (float startVal, float endVal, float tolerance, float slewRate);

private:
    float generateNextValue () override;

private:
    float currentRate;
};

} // namespace friz
