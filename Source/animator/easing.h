/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatedValue.h" 


namespace friz 
{
class EasingCurve : public ToleranceValue 
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
   /**
    * Decelerate into the end value. 
    * @param startVal  start value 
    * @param endVal    end value 
    * @param tolerance Tolerance for stopping. 
    * @param slewRate  slew rate, must be 0 < rate < 1
    */
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
   /**
    * Accelerate into the end value. 
    * @param startVal  start val 
    * @param endVal    end val 
    * @param tolerance tolerance for stopping 
    * @param slewRate  slew rate, must be > 1. 
    */
   EaseOut(int startVal, int endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;

private:
   float fCurrentRate; 
   
};


} // namespace friz
