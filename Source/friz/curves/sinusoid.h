/*
 * Copyright (c) 2019 Brett g Porter.
 */

#pragma once

#include "animatedValue.h"

namespace friz
{
/**
 * @class Sinusoid
 * @brief Sine/Cosine wave generator.
 *
 * Note that this object always generates values in the natural range of
 * a sinusoid, -1 .. +1. Your code will need to perform whatever scaling or
 * offsets required for things to make sense in your use case.
 */

class Sinusoid : public TimedValue
{
public:
    /**
     * A value object that can output sinusoid values. Note that we
     * place very few constraints here for folks who like to work
     * the corner conditions.
     * @param startPhase start phase, typically 0..2pi
     * @param endPhase   end phase, typically 0..2pi.
     * @param duration   duration in frames.
     */
    Sinusoid (float startPhase, float endPhase, int duration)
    : TimedValue (startPhase, endPhase, duration)
    {
        jassert (duration > 0);

        // requesting the same start/end phase is a request to
        // generate a single cycle
        if (std::abs (endVal - startVal) < 0.01f)
        {
            endVal = startVal + juce::MathConstants<float>::twoPi;
        }

        // if the end phase is less than start, adjust.
        while (endVal < startVal)
        {
            endVal += juce::MathConstants<float>::twoPi;
        }

        phaseDelta = (endVal - startVal) / (duration - 1);
        phase      = startVal;
        // hack to make 1st value correct: all of the other value generators
        // correctly use 'startVal` as the first value to be returned. This
        // value generator does *not* want that; startVal is the phase of the
        // sinusoid, and we need to make sure that we convert phase back into
        // the actual sinusoid value on 1st call (and this went undetected for
        // so long because we were only testing with start phase = 0.)
        startVal = std::sin (phase);
        // also update the end val in case the animation is cancelled and we jump
        // to the end.
        endVal = std::sin (endVal);
    }

    /**
     * Simpler interface to create Sinusoid objects -- specify start
     * and end positions in terms of integer multiples of pi/2. To create a full
     * sine wave, specify start = 0 end = 4; for a full cosine wave use
     * start = 1, end = 5.
     * @param startQuad starting quadrant
     * @param endQuad   ending quadrant
     * @param duration  duration in frames.
     */

    Sinusoid (int startQuad, int endQuad, int duration)
    : Sinusoid (startQuad * juce::MathConstants<float>::halfPi,
                endQuad * juce::MathConstants<float>::halfPi, duration)
    {
    }

private:
    float GenerateNextValue () override
    {
        phase += phaseDelta;
        return std::sin (phase);
    }

private:
    /// current phase accumulator.
    float phase;

    /// phase increment for each frame.
    float phaseDelta;
};

} // namespace friz
