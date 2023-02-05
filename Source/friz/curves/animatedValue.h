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

namespace friz
{

/**
 * @class AnimatedValue
 * @brief Abstract base class for objects that can generate a useful series
 *        of values to drive UI animations.
 */

class AnimatedValue
{
public:
    /**
     * @brief Base class init for the animated value classes.
     * @param startVal  Initial Value
     * @param endVal    Target/end value.
     */
    AnimatedValue (float startVal_, float endVal_)
    : startVal { startVal_ }
    , endVal { endVal_ }
    , currentVal { startVal_ } {

    };

    virtual ~AnimatedValue () = default;

    /**
     * Derived classes should do whatever is needed to generate and
     * return the next value.
     * @param   msElapsed time since this value started running. (used by 'time-based'
     *          values)
     * @param   msSinceLastUpdate time since we were last evaluated (used by
     *          threshold-based values)
     * @return        next value (or last value if we're finished)
     */
    virtual float getNextValue (int msElapsed, int msSinceLastUpdate) = 0;

    /**
     * @brief get the ending state of this value object. When we cancel
     * an in-progress animation, we may need to snap to the end value, and
     * this gives a way to get there immediately.
     *
     * @return float
     */
    float getEndValue () const { return endVal; }

    /**
     * Have we reached the end of this animation sequence? By default,
     * we're done when the current value is within `tolerance` of the endValue
     * (or if we've been canceled...)
     * @return true if this value has reached the end of its animation.
     */
    virtual bool isFinished () = 0;

    /**
     * @brief Attempt to change the end value of an animation that's currently in process.
     *
     * @param newValue
     * @return true If the value type supports this and the operation succeeded.
     */
    virtual bool updateTarget (float /*newValue*/) { return false; }

    /**
     * @brief Cancel an in-progress animation.
     *
     * @param moveToEndPosition If true, will immediately take the ending value; otherwise
     * cancels at its current value.
     */
    void cancel (bool moveToEndPosition)
    {
        if (!canceled)
        {
            canceled = true;
            doCancel (moveToEndPosition);
        }
    }

private:
    /**
     * Override in derived classes to perform any unusual cancellation logic.
     */
    virtual void doCancel (bool moveToEndPosition)
    {
        if (moveToEndPosition)
            currentVal = endVal;
    }

protected:
    float startVal;
    float endVal;
    float currentVal;

    bool canceled { false };
    bool finished { false };
};

class ToleranceValue : public AnimatedValue
{
public:
    ToleranceValue (float startVal, float endVal, float tolerance)
    : AnimatedValue { startVal, endVal }
    , tolerance { tolerance }
    {
    }

    /**
     * @brief Calculate the next value in the sequence based on the delta
     * time since last updated. Internally, we use an update rate of
     * 1 kHz to recalculate values so that we can remain consistent
     * as the actual animation frame rate changes.
     * @param msSinceLastUpdate a delta time since last updated.
     * @return the calculated value.
     */
    float getNextValue (int /*msElapsed*/, int msSinceLastUpdate) override
    {
        jassert (msSinceLastUpdate >= 0);
        if (msSinceLastUpdate == 0)
            return currentVal;

        for (int i { 0 }; i < msSinceLastUpdate; ++i)
        {
            currentVal = snapToEnd (generateNextValue ());
            if (isFinished ())
                break;
        }

        return currentVal;
    }

    /**
     * @brief Test to see if this value has reached its end state.
     */
    bool isFinished () override
    {
        // we are finished in either of these cases:
        // 1. user/code canceled us
        // 2. current value is within tolerance of the end value.
        return (finished || canceled);
    }

private:
    /**
     * @brief The underlying calculation (in floating point) may approach the
     * desired end value asymptotically; we've already defined a tolerance
     * value that's "close enough" to the end. When we get inside that tolerance,
     * we snap to the end value and mark this object as finished.
     *
     */
    float snapToEnd (float val)
    {
        if (std::fabs (val - endVal) < tolerance)
        {
            finished = true;
            return endVal;
        }
        return val;
    }

    /**
     * @brief Execute a single step of this curve's function.
     *
     * @return      next value.
     */
    virtual float generateNextValue () = 0;

protected:
    float tolerance;
};

class TimedValue : public AnimatedValue
{
public:
    TimedValue (float startVal, float endVal, int duration_)
    : AnimatedValue { startVal, endVal }
    , duration { duration_ }
    {
    }

    float getNextValue (int msElapsed, int /* msSinceLastUpdate*/) override
    {
        if (msElapsed >= duration)
        {
            finished   = true;
            currentVal = endVal;
            return currentVal;
        }
        float progress { static_cast<float> (msElapsed) / duration };
        return generateNextValue (progress);
    }

    bool isFinished () override { return finished; }

protected:
    /**
     * @brief Given a fractional curve point (typically) in the range (0.f..1.f),
     * interpolate this point between this value's start and end points.
     *
     * @param curvePoint
     * @return float
     */
    float scale (float curvePoint) { return startVal + curvePoint * (endVal - startVal); }

private:
    /**
     * @brief generate the value according to progress in time.
     * @param progress position in the animation (0.0..1.0)
     *
     * @return      next value.
     */
    virtual float generateNextValue (float progress) = 0;

protected:
    /// @brief duration of the event in ms.
    int duration;
};

// GCC doesn't support some functions that are specified in the standard:
// std::sinf / cosf/ powf/ etc. (see
// https://stackoverflow.com/questions/56417980/cosf-sinf-etc-are-not-in-std).
// We define a few inline utility functions to narrow the return type of
// the double-returning sin/cos/pow functions to float as we prefer here.
// Thanks to [Sudara](https://github.com/sudara) for catching this.

inline float sin_f (float v)
{
    return static_cast<float> (std::sin (v));
}

inline float cos_f (float v)
{
    return static_cast<float> (std::cos (v));
}

inline float pow_f (float v, float pow)
{
    return static_cast<float> (std::pow (v, pow));
}

} // namespace friz
