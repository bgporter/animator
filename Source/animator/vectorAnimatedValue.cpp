/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "vectorAnimatedValue.h"

VectorAnimatedValue::VectorAnimatedValue(int startVal, int endVal, float tolerance, 
   float accel, float damping)
:  AnimatedValue(startVal, endVal, tolerance)
,  fStartAcceleration(accel)
,  fAcceleration(accel)
,  fDamping(damping)
,  fVelocity(0)
{
   // the damping factor must be between 0..1. 
   jassert(isPositiveAndBelow(damping, 1.f));
   if (endVal < startVal)
   {
      fAcceleration *= -1;
   }
   
}


float VectorAnimatedValue::GenerateNextValue()
{
   // a weird case we need to handle -- if we're already at our end state, 
   // remain there! Dont' keep oscillating. 
   if (this->IsFinished())
   {
      return fEndVal;
   }
   
   float prev = fCurrentVal;
   fVelocity += fAcceleration;
   float next = prev + fVelocity;
   
   
   if (next > fEndVal)
   {
      int zz = 0;
   }
   
   // see if we have crossed over the end value
   // if so, we may need to change direction and dampen the oscillation
   if ((fEndVal - prev) * (fEndVal - next) <= 0)
   {
      if (0.f == fDamping)
      {
         // we overshot the end and don't want to oscillate. Clamp to the 
         // end value instead. 
         next = fEndVal;
      }
      else 
      {
         // reverse our acceleration direction and reduce it by the 
         // damping factor; we'll keep oscillating around the end value 
         // until we're within the tolerance value of it, at which 
         // point we'll stop.
         fAcceleration = -1 * fAcceleration * fDamping;
         fVelocity = 0.f;
          
      }
   }
   
   return next;
   
   
   
}

void VectorAnimatedValue::DoReset()
{
   fVelocity = 0;
   fAcceleration = fStartAcceleration;
}
   
   
   
#ifdef qRunUnitTests
#include "test/test_VectorAnimatedValue.cpp"
#endif
