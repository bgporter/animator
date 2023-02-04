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
}

float Linear::generateNextValue (float progress)
{
    // simple linear interpolation; maybe use the parametric
    // curves instead?
    return scale (progress);
}

#ifdef qRunUnitTests
#include "test/test_Linear.cpp"
#endif

} // namespace friz
