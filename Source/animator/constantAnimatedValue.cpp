/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "constantAnimatedValue.h"
   
   
   
ConstantAnimatedValue::ConstantAnimatedValue(int value, int duration)
:  AnimatedValue(value, value, 0)
,  fDuration(duration)
{
   
}

bool ConstantAnimatedValue::IsFinished()
{
   return (fFrameCount >= fDuration);
}

float ConstantAnimatedValue::GenerateNextValue()
{
   return fStartVal;
}


void ConstantAnimatedValue::DoReset()
{
   // fElapsedTime = 0;
}

#ifdef qRunUnitTests
#include "test/test_ConstantAnimatedValue.cpp"
#endif
