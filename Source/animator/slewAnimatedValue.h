/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatedValue.h" 

class SlewAnimatedValue : public AnimatedValue 
{
public:
   SlewAnimatedValue(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
protected:
   float fSlewRate;
};


class SlewAccelerateAnimatedValue : public SlewAnimatedValue 
{
public:
   SlewAccelerateAnimatedValue(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
};
