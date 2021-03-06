/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "linearAnimatedValue.h"



LinearAnimatedValue::LinearAnimatedValue(float startVal, float endVal, 
   float tolerance, int duration)
:  AnimatedValue(startVal, endVal, tolerance) 
,  fDuration(duration) 
{
   jassert(duration > 0);
   fDelta = (fEndVal - fStartVal) / fDuration;
}



float LinearAnimatedValue::GenerateNextValue()
{
   auto calculated = fCurrentVal + fDelta;
   if (fStartVal < fEndVal)
   {
      return jmin(calculated, fEndVal);
   }
   return jmax(calculated, fEndVal);
}

#ifdef qRunUnitTests
#include "test/test_LinearAnimatedValue.cpp"
#endif
