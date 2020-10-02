/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#pragma once


#include "animatedValue.h"

class LinearAnimatedValue  : public AnimatedValue 
{
public:
   /**
    * A value that changes linearly. 
    * @param startVal  initial value 
    * @param endVal    ending value 
    * @param tolerance tolerance for completion. 
    * @param duration  # of frames the effect should take. 
    */
   LinearAnimatedValue(float startVal, float endVal, float tolerance, int duration);
   
   
   
private:
   float GenerateNextValue() override;

private:
   
   /// duration of this effect in frames. 
   int fDuration;
   
   
   /// Delta to add at each millisecond that passes
   float fDelta;
   
};