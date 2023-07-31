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

#include "easing.h"

namespace friz
{

EasingCurve::EasingCurve (float startVal_, float endVal_, float tolerance_, float slewRate_)
: ToleranceValue (startVal_, endVal_, tolerance_)
, slewRate (slewRate_)
{
}

EaseIn::EaseIn (float startVal_, float endVal_, float tolerance_, float slewRate_)
: EasingCurve (startVal_, endVal_, tolerance_, slewRate_)
{
    jassert (slewRate < 1.f);
}

float EaseIn::generateNextValue ()
{
    return currentVal + slewRate * (endVal - currentVal);
}

EaseOut::EaseOut (float startVal_, float endVal_, float tolerance_, float slewRate_)
: EasingCurve (startVal_, endVal_, tolerance_, slewRate_)
, currentRate { 0.01f }
{
    jassert (slewRate > 1.f);
}

float EaseOut::generateNextValue ()
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
