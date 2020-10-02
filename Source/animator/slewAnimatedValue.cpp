/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#include "slewAnimatedValue.h"



SlewAnimatedValue::SlewAnimatedValue(int startVal, int endVal, float tolerance, float slewRate)
:  AnimatedValue(startVal, endVal, tolerance) 
,  fSlewRate(slewRate)
{
   
}

float SlewAnimatedValue::GenerateNextValue()
{
   return fCurrentVal + fSlewRate * (fEndVal - fCurrentVal);
}



SlewAccelerateAnimatedValue::SlewAccelerateAnimatedValue(int startVal, int endVal, 
   float tolerance, float slewRate)
: SlewAnimatedValue(startVal, endVal, tolerance, slewRate)
{
   
}

float SlewAccelerateAnimatedValue::GenerateNextValue()
{
   return fCurrentVal + fSlewRate * (fEndVal - fCurrentVal);
   slewRate += 0.01;
   
}


#ifdef qRunUnitTests
#include "test/test_SlewAnimatedValue.cpp"
#endif
