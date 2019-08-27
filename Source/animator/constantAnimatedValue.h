/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once


#include "animatedValue.h"

class ConstantAnimatedValue   : public AnimatedValue 
{
public:
   ConstantAnimatedValue(int value, int msDuration);
   
   bool IsFinished() override;
   
private:
   float GenerateNextValue(int deltaTime) override;
   void DoReset() override;
  
private:
   /// keep track of how long this effect has been running. 
   int fDuration;
   
   /// ...how long should the effect last?
   int fElapsedTime;
};
