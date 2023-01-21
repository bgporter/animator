/*
 * Copyright (c) 2019 Brett g Porter.
 */

#include "easing.h"

namespace friz
{

EasingCurve::EasingCurve (float startVal, float endVal, float tolerance, float slewRate_)
: ToleranceValue (startVal, endVal, tolerance)
, slewRate (slewRate_)
{
}

EaseIn::EaseIn (float startVal, float endVal, float tolerance, float slewRate)
: EasingCurve (startVal, endVal, tolerance, slewRate)
{
    jassert (slewRate < 1.f);
}

float EaseIn::GenerateNextValue ()
{
    return currentVal + slewRate * (endVal - currentVal);
}

EaseOut::EaseOut (float startVal, float endVal, float tolerance, float slewRate)
: EasingCurve (startVal, endVal, tolerance, slewRate)
, currentRate { 0.01f }
{
    jassert (slewRate > 1.f);
}

float EaseOut::GenerateNextValue ()
{
    auto val = currentVal + currentRate * (endVal - currentVal);

    // limit the slew to prevent us blowing up.
    currentRate = std::min (0.95f, currentRate * slewRate);
    return val;
}

#ifdef qRunUnitTests
#include "test/test_Easing.cpp"
#endif

} // namespace friz
