/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "constantAnimatedValue.h"
   
   
   
ConstantAnimatedValue::ConstantAnimatedValue(int value, int msDuration)
:  AnimatedValue(value, value, 0)
,  fDuration(msDuration)
,  fElapsedTime(0)
{
   
}

bool ConstantAnimatedValue::IsFinished()
{
   return (fElapsedTime >= fDuration);
}

float ConstantAnimatedValue::GenerateNextValue(int deltaTime)
{
   fElapsedTime += deltaTime;
   return fStartVal;
}


void ConstantAnimatedValue::DoReset()
{
   fElapsedTime = 0;
}

#ifdef qRunUnitTests
#include "test/test_ConstantAnimatedValue.cpp"
#endif
