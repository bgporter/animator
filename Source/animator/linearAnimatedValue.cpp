/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "linearAnimatedValue.h"



LinearAnimatedValue::LinearAnimatedValue(float startVal, float endVal, 
   float tolerance, int msDuration)
:  AnimatedValue(startVal, endVal, tolerance) 
,  fDuration(msDuration) 
{
   jassert(msDuration > 0);
   fDelta = (fEndVal - fStartVal) / fDuration;
}



float LinearAnimatedValue::GenerateNextValue(int deltaTime)
{
   return fCurrentVal + (deltaTime * fDelta);
}

#ifdef qRunUnitTests
#include "test/test_LinearAnimatedValue.cpp"
#endif
