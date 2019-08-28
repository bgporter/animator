/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatedValue.h" 

class VectorAnimatedValue  : public AnimatedValue 
{
public:
   VectorAnimatedValue(int startVal, int endVal, float tolerance, float accel, float dampen);
   
   
   
private:
   float GenerateNextValue() override;
   
   void DoReset() override;
   
private:
   /// The initial acceleration for this value. 
   float fStartAcceleration;
   
   /// When we're in a damping state, the acceleration will change. 
   float fAcceleration;
   
   /// damping factor to apply when we need to oscillate toward end val. 
   float fDamping;
   
   /// current velocity, updated with acceleration every frame. 
   float fVelocity;
};
