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

float Constant::GenerateNextValue ()
{
    return fStartVal;
}

#ifdef qRunUnitTests
#include "test/test_Constant.cpp"
#endif

} // namespace friz
