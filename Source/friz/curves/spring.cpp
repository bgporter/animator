/*
 * Copyright (c) 2019 Brett g Porter.
 */
#include "spring.h"

namespace friz
{
Spring::Spring (float startVal, float endVal, float tolerance, float accel,
                float damping_)
: ToleranceValue (startVal, endVal, tolerance)
, startAcceleration (accel)
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
        if (0.f == damping)
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
