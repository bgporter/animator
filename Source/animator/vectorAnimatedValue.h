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
   float fAcceleration;
   
   float fDamping;
   
   float fVelocity;
};
