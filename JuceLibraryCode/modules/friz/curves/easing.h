/*
 * Copyright (c) 2019 Brett g Porter. 
 */

#pragma once

#include "animatedValue.h" 


namespace friz 
{
class EasingCurve : public ToleranceValue 
{
public:
   EasingCurve(float startVal, float endVal, float tolerance, float slewRate);
   
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
   EaseIn(float startVal, float endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;
   
};

class SmoothedValue : public EaseIn 
{
    SmoothedValue(float startVal, float endVal, float tolerance, float slewRate)
    :   EaseIn(startVal, endVal, tolerance, slewRate)
    {
        
    }

    /**
     * @brief Update the target value while the animation is running. 
     * 
     * @param newTarget 
     */
    void UpdateTarget(float newTarget) override
    {
        fEndVal = newTarget;
    }    
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
   EaseOut(float startVal, float endVal, float tolerance, float slewRate);
   
private:
   float GenerateNextValue() override;

private:
   float fCurrentRate; 
   
};


} // namespace friz
