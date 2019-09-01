/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "constant.h"
   
   
   
Constant::Constant(int value, int duration)
:  AnimatedValue(value, value, 0)
,  fDuration(duration)
{
   
}

bool Constant::IsFinished()
{
   return (fFrameCount >= fDuration);
}

float Constant::GenerateNextValue()
{
   return fStartVal;
}


void Constant::DoReset()
{
   // fElapsedTime = 0;
}

#ifdef qRunUnitTests
#include "test/test_Constant.cpp"
#endif
