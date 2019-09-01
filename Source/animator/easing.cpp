/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#include "easing.h"



EaseIn::EaseIn(int startVal, int endVal, float tolerance, float slewRate)
:  AnimatedValue(startVal, endVal, tolerance) 
,  fSlewRate(slewRate)
{
   
}

float EaseIn::GenerateNextValue()
{
   return fCurrentVal + fSlewRate * (fEndVal - fCurrentVal);
}



EaseOut::EaseOut(int startVal, int endVal, 
   float tolerance, float slewRate)
: EaseIn(startVal, endVal, tolerance, slewRate)
{
   
}

float EaseOut::GenerateNextValue()
{
   auto val =  fCurrentVal + fSlewRate * (fEndVal - fCurrentVal);
   fSlewRate *= 1.3;
   return val;
   
}


#ifdef qRunUnitTests
#include "test/test_Easing.cpp"
#endif
