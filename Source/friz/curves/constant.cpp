/*
 * Copyright (c) 2019 Brett g Porter.
 */
#include "constant.h"

namespace friz
{

Constant::Constant (float value, int duration)
: TimedValue (value, value, duration)
{
}

float Constant::generateNextValue (float /*progress*/)
{
    return startVal;
}

#ifdef qRunUnitTests
#include "test/test_Constant.cpp"
#endif

} // namespace friz
