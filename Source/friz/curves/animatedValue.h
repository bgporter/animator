/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
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
    , currentVal { startVal_ }
    , frameCount { 0 }
    , canceled { false } {

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
    float GetNextValue (int msElapsed, int msSinceLastUpdate)
    {
        if (!canceled)
        {
            if (0 == frameCount++)
                currentVal = startVal;
            else
                currentVal = SnapToTolerance (GenerateNextValue ());
        }

        return currentVal;
    }

    /**
     * Have we reached the end of this animation sequence? By default,
     * we're done when the current value is within `tolerance` of the endValue
     * (or if we've been canceled...)
     * @return true if this value has reached the end of its animation.
     */
    virtual bool IsFinished () = 0;

    /**
     * @brief Attempt to change the end value of an animation that's currently in process.
     *
     * @param newValue
     * @return true If the value type supports this and the operation succeeded.
     */
    virtual bool UpdateTarget (float /*newValue*/) { return false; }

    /**
     * @brief Cancel an in-progress animation.
     *
     * @param moveToEndPosition If true, will immediately take the ending value; otherwise
     * cancels at its current value.
     */
    void Cancel (bool moveToEndPosition)
    {
        if (!canceled)
        {
            canceled = true;
            DoCancel (moveToEndPosition);
        }
    }

private:
    /**
     * Implemented in derived classes to generate the next value in the sequence.
     * @return      next value.
     */
    virtual float GenerateNextValue () = 0;

    /**
     * @brief Some derived classes should snap to the end value when within
     *        some tolerance of it. Default implementation does nothing.
     *
     * @return (possibly modified) value
     */
    virtual float SnapToTolerance (float val) { return val; }

    /**
     * Override in derived classes to perform any unusual cancellation logic.
     */
    virtual void DoCancel (bool moveToEndPosition)
    {
        if (moveToEndPosition)
            currentVal = endVal;
    }

protected:
    float startVal;
    float endVal;
    float currentVal;

    int frameCount;
    bool canceled { false };
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
     * @brief If the current value is within tolerance of the end value,
     *        snap the current value to the end value and return true
     *        to indicate that we did this. This prevents us from stopping
     *        shy of the actual desired end value.
     *
     * @return true
     * @return false
     */
    float SnapToTolerance (float val) override
    {
        if (ValueIsWithinTolerance (val))
            return endVal;
        return val;
    }

    bool ValueIsWithinTolerance (float val) const
    {
        return (std::fabs (val - endVal) < tolerance);
    }

    bool IsFinished () override
    {
        // we are finished in either of these cases:
        // 1. user/code canceled us
        // 2. current value is within tolerance of the end value.
        return (ValueIsWithinTolerance (currentVal) || canceled);
    }

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

    bool IsFinished () override { return frameCount >= duration; }

protected:
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
