/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "vectorAnimatedValue.h"

VectorAnimatedValue::VectorAnimatedValue(int startVal, int endVal, float tolerance, 
   float accel, float damping)
:  AnimatedValue(startVal, endVal, tolerance)
,  fAcceleration(accel)
,  fDamping(damping)
,  fVelocity(0)
{
   
}


float VectorAnimatedValue::GenerateNextValue()
{
   
}

void VectorAnimatedValue::DoReset()
{
   fVelocity = 0;
}
   
   
   
#ifdef qRunUnitTests
#include "test/test_VectorAnimatedValue.cpp"
#endif
