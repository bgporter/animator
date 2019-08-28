/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#include "slewAnimatedValue.h"



SlewAnimatedValue::SlewAnimatedValue(int startVal, int endVal, float tolerance, float slewRate)
:  AnimatedValue(startVal, endVal, tolerance) 
,  fSlewRate(slewRate)
{
   
}

float SlewAnimatedValue::GenerateNextValue(int deltaTime)
{
   return fCurrentVal + fSlewRate * (fEndVal - fCurrentVal);
}

#ifdef qRunUnitTests
#include "test/test_SlewAnimatedValue.cpp"
#endif
