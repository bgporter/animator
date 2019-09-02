/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatedValue.h" 


/**
 * @class Spring
 * @brief Accelerates toward the end value -- if this acceleration takes it past 
 *        the end value, it enters a mode where it acts like a simplified 
 *        oscillating spring; the behavior of the oscillation can be loosened or 
 *        tightened using the `dampen` parameter. 
 */

class Spring  : public AnimatedValue 
{
public:
   Spring(int startVal, int endVal, float tolerance, float accel, float dampen);
   
   
   
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
