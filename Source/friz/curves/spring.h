/*
 * Copyright (c) 2019 Brett g Porter.
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
    /// The initial acceleration for this value.
    float fStartAcceleration;

    /// When we're in a damping state, the acceleration will change.
    float fAcceleration;

    /// damping factor to apply when we need to oscillate toward end val.
    float fDamping;

    /// current velocity, updated with acceleration every frame.
    float fVelocity;
};

} // namespace friz
