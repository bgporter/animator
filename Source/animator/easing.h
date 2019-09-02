/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatedValue.h" 


class EasingCurve : public AnimatedValue 
{
public:
   EasingCurve(int startVal, int endVal, float tolerance, float slewRate);
   
protected:
   float fSlewRate;
   
};


/**
 * @class EaseIn 
 *
 * @brief A slew-based ease in curve -- accelerates quickly, then decelerates 
 *        as it approaches the end value. 
 * 
 */

class EaseIn : public EasingCurve
{
public:
   EaseIn(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
};

/**
 * @class EaseOut 
 *
 * @brief A slew-based acceleration. starts slowly & accelerates. 
 */
class EaseOut :public  EasingCurve
{
public:
   EaseOut(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
};
