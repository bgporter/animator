/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once


#include "animatedValue.h"

class ConstantAnimatedValue   : public AnimatedValue 
{
public:
   /**
    * A value that doesn't change. 
    * @param value      Value to generate. 
    * @param duration # of frames the effect should take. 
    */
   ConstantAnimatedValue(int value, int duration);
   
   bool IsFinished() override;
   
private:
   float GenerateNextValue() override;
   void DoReset() override;
  
private:
   /// keep track of how long this effect has been running. 
   int fDuration;
   
};
