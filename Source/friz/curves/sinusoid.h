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
    Sinusoid (float startPhase_, float endPhase_, int duration)
    : TimedValue (startPhase_, endPhase_, duration)
    , startPhase { startPhase_ }
    , endPhase { endPhase_ }
    {
        jassert (duration > 0);

        // requesting the same start/end phase is a request to
        // generate a single cycle
        if (std::abs (endPhase - startPhase) < 0.01f)
        {
            endPhase = startPhase + juce::MathConstants<float>::twoPi;
        }

        // if the end phase is less than start, adjust.
        while (endPhase < startPhase)
        {
            endPhase += juce::MathConstants<float>::twoPi;
        }

        // hack to make 1st value correct: all of the other value generators
        // correctly use 'startVal` as the first value to be returned. This
        // value generator does *not* want that; startVal is the phase of the
        // sinusoid, and we need to make sure that we convert phase back into
        // the actual sinusoid value on 1st call (and this went undetected for
        // so long because we were only testing with start phase = 0.)
        startVal = std::sin (startPhase);
        // also update the end val in case the animation is cancelled and we jump
        // to the end.
        endVal = std::sin (endPhase);
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
    /**
     * @brief Calculate the next phase value, then return its sine as the
     * actual curve value.
     *
     * @param progress
     * @return float
     */
    float generateNextValue (float progress) override
    {
        const auto phase { startPhase + (progress * (endPhase - startPhase)) };
        return std::sin (phase);
    }

private:
    /// @brief initial phase of the sinusoid.
    float startPhase;
    /// @brief ending phase of the sinusoid
    float endPhase;
};

} // namespace friz
