/*
 * Copyright (c) 2019 Brett g Porter.
 */
#include "linear.h"

namespace friz
{

Linear::Linear (float startVal, float endVal, int duration)
: TimedValue (startVal, endVal, duration)
{
    jassert (duration > 0);
    delta = (endVal - startVal) / duration;
}

float Linear::generateNextValue ()
{
    const auto calculated = currentVal + delta;
    if (startVal < endVal)
        return juce::jmin (calculated, endVal);
    return juce::jmax (calculated, endVal);
}

#ifdef qRunUnitTests
#include "test/test_Linear.cpp"
#endif

} // namespace friz
