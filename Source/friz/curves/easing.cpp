/*
 * Copyright (c) 2019 Brett g Porter.
 */

#include "easing.h"

namespace friz
{

EasingCurve::EasingCurve (float startVal, float endVal, float tolerance, float slewRate)
: ToleranceValue (startVal, endVal, tolerance)
, fSlewRate (slewRate)
{
}

EaseIn::EaseIn (float startVal, float endVal, float tolerance, float slewRate)
: EasingCurve (startVal, endVal, tolerance, slewRate)
{
    jassert (slewRate < 1.f);
}

float EaseIn::GenerateNextValue ()
{
    return fCurrentVal + fSlewRate * (fEndVal - fCurrentVal);
}

EaseOut::EaseOut (float startVal, float endVal, float tolerance, float slewRate)
: EasingCurve (startVal, endVal, tolerance, slewRate)
, fCurrentRate { 0.01f }
{
    jassert (slewRate > 1.f);
}

float EaseOut::GenerateNextValue ()
{
    auto val = fCurrentVal + fCurrentRate * (fEndVal - fCurrentVal);

    // limit the slew to prevent us blowing up.
    fCurrentRate = std::min (0.95f, fCurrentRate * fSlewRate);
    return val;
}

#ifdef qRunUnitTests
#include "test/test_Easing.cpp"
#endif

} // namespace friz
