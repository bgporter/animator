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
     * Base class init for the animated value classes.
     * @param startVal  Initial Value
     * @param endVal    Target/end value.
     * @param tolerance Toleerance for when we've reached the target value.
     */
    AnimatedValue (float startVal, float endVal)
    : fStartVal (startVal)
    , fEndVal (endVal)
    , fCurrentVal (startVal)
    , fFrameCount (0)
    , fCanceled (false) {

    };

    virtual ~AnimatedValue () {

    };

    /**
     * Derived classes should do whatever is needed to generate and
     * return the next value.
     * @return        next value (or last value if we're finished)
     */
    float getNextValue ()
    {
        if (!fCanceled)
        {
            if (0 == fFrameCount++)
            {
                fCurrentVal = fStartVal;
            }
            else
            {
                fCurrentVal = snapToTolerance (generateNextValue ());
            }
        }

        return fCurrentVal;
    }

    /**
     * Have we reached the end of this animation sequence? By default,
     * we're done when the current value is within `fTolerance` of the endValue
     * (or if we've been canceled...)
     * @return [description]
     */
    virtual bool isFinished () = 0;

    /**
     * @brief Attempt to change the end value of an animation that's currently in process.
     *
     * @param newValue
     * @return true If the value type supports this and the operation succeeded.
     * @return false
     */
    virtual bool updateTarget (float /*newValue*/) { return false; }

    void cancel (bool moveToEndPosition)
    {
        fCanceled = true;
        doCancel (moveToEndPosition);
    }

private:
    /**
     * Implemented in derived classes to generate the next value in the sequence.
     * @return      next value.
     */
    virtual float generateNextValue () = 0;

    /**
     * @brief Some derived classes should snap to the end value when within
     *        some tolerance of it. Default implementation does nothing.
     *
     * @return (possibly modified) value
     */
    virtual float snapToTolerance (float val) { return val; }

    /**
     * Override in derived classes to perform any unusual cancellation logic.
     */
    virtual void doCancel (bool moveToEndPosition)
    {
        if (moveToEndPosition)
        {
            fCurrentVal = fEndVal;
        }
    }

protected:
    float fStartVal;
    float fEndVal;
    float fCurrentVal;

    int fFrameCount;
    bool fCanceled;

private:
};

class ToleranceValue : public AnimatedValue
{
public:
    ToleranceValue (float startVal, float endVal, float tolerance)
    : AnimatedValue (startVal, endVal)
    , fTolerance (tolerance)
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
    float snapToTolerance (float val) override
    {
        if (valueIsWithinTolerance (val))
        {
            return fEndVal;
        }
        return val;
    }

    bool valueIsWithinTolerance (float val) const
    {
        return (std::fabs (val - fEndVal) < fTolerance);
    }

    bool isFinished () override
    {
        // we are finished in either of these cases:
        // 1. user/code canceled us
        // 2. current value is within tolerance of the end value.
        return (valueIsWithinTolerance (fCurrentVal) || fCanceled);
    }

protected:
    float fTolerance;
};

class TimedValue : public AnimatedValue
{
public:
    TimedValue (float startVal, float endVal, int duration)
    : AnimatedValue (startVal, endVal)
    , fDuration (duration)
    {
    }

    bool isFinished () override { return fFrameCount >= fDuration; }

protected:
    int fDuration;
};

} // namespace friz
