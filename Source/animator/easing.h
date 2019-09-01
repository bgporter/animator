/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatedValue.h" 

class EaseIn : public AnimatedValue 
{
public:
   EaseIn(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
protected:
   float fSlewRate;
};


class EaseOut : public EaseIn 
{
public:
   EaseOut(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
};
