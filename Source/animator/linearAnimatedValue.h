/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#pragma once


#include "animatedValue.h"

class LinearAnimatedValue  : public AnimatedValue 
{
public:
   LinearAnimatedValue(float startVal, float endVal, float tolerance, int msDuration);
   
   
   
private:
   float GenerateNextValue(int deltaTime) override;

private:
   
   /// duration of this effect in ms. 
   int fDuration;
   
   
   /// Delta to add at each millisecond that passes
   float fDelta;
   
};