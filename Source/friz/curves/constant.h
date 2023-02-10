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
 * @class Constant
 * @brief A class that generates a single constant value for its duration.
 *
 * Wait -- why would you need a constant value when you're trying to *animate*?
 *
 * Consider this case: I have an an animation that's cyclic, and I want to be
 * able to set it to run for some number of cycles that I don't know at compile
 * time. An easy way to do this is to create an Animation object that has a
 * `Constant` value in it, and in the `OnCompletion()` callback, check to see if
 * that value is > 0 -- if not, immediately recreate the animation, but decrement
 * the constant loop count value.
 */

class Constant : public TimedValue
{
public:
    /**
     * A value that doesn't change.
     * @param value      Value to generate.
     * @param duration # of milliseconds the effect should take.
     */
    Constant (float value, int duration);

    /**
     * @brief An alternate constructor that can be used by the
     * `makeAnimation()` factory function, which requires that all
     * animation effects have separate start and end values. We ignore anything
     * passed to the startValue argument.
     *
     * @param startVal **ignored**
     * @param endVal    The only value this effect wil emit.
     * @param duration in milliseconds.
     *
     */
    Constant (float /*startVal*/, float endVal, int duration);

private:
    float generateNextValue (float progress) override;

private:
};

} // namespace friz
