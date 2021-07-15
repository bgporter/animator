/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once


namespace friz
{

/**
 * @class AnimatedValue
 * @brief Abstract base class for objects that can generate a useful series
 *        of values to drive UI animations. 
 */

class AnimatedValue 
{
public:
   /**
    * Base class init for the animated value classes. 
    * @param startVal  Initial Value 
    * @param endVal    Target/end value. 
    * @param tolerance Toleerance for when we've reached the target value.
    */
   AnimatedValue(float startVal, float endVal)
   :  fStartVal(startVal)
   ,  fEndVal(endVal)
   ,  fCurrentVal(startVal)
   ,  fFrameCount(0)
   ,  fCanceled(false)
   {
      
   };
   
   virtual ~AnimatedValue()
   {
      
   };
   
   /**
    * Derived classes should do whatever is needed to generate and 
    * return the next value. 
    * @return        next value (or last value if we're finished)
    */
   float GetNextValue()
   {
      if (! fCanceled)
      {
         if (0 == fFrameCount++)
         {
            fCurrentVal = fStartVal;
         }
         else 
         {
            fCurrentVal = this->SnapToTolerance(this->GenerateNextValue());
         }
      }
      
      return fCurrentVal;
   }
   
   /**
    * Have we reached the end of this animation sequence? By default, 
    * we're done when the current value is within `fTolerance` of the endValue
    * (or if we've been canceled...)
    * @return [description]
    */
   virtual bool IsFinished() = 0;
   

   void Cancel(bool moveToEndPosition)
   {
      fCanceled = true;
      this->DoCancel(moveToEndPosition);
   }


private:
   
   /**
    * Implemented in derived classes to generate the next value in the sequence. 
    * @return      next value.
    */
   virtual float GenerateNextValue() = 0;

   /**
    * @brief Some derived classes should snap to the end value when within 
    *        some tolerance of it. Default implementation does nothing. 
    * 
    * @return (possibly modified) value 
    */
   virtual float SnapToTolerance(float val) { return val; }
   
   
   /**
    * Override in derived classes to perform any unusual cancellation logic. 
    */
   virtual void DoCancel(bool moveToEndPosition)
   {
      if (moveToEndPosition)
      {
         fCurrentVal = fEndVal;
      }
   }


protected: 
   float fStartVal;
   float fEndVal;
   float fCurrentVal;
   
   int fFrameCount;
   bool fCanceled;
private:
   
};


class ToleranceValue : public AnimatedValue 
{
public:
   ToleranceValue(float startVal, float endVal, float tolerance)
   :  AnimatedValue(startVal, endVal)
   ,  fTolerance(tolerance)
   {
      
   }


   /**
    * @brief If the current value is within tolerance of the end value, 
    *        snap the current value to the end value and return true
    *        to indicate that we did this. This prevents us from stopping 
    *        shy of the actual desired end value. 
    * 
    * @return true 
    * @return false 
    */
   float SnapToTolerance(float val) override
   {
      if (this->ValueIsWithinTolerance())
      {
         return fEndVal;
      }
      return val;
   }

   bool ValueIsWithinTolerance() const 
   {
      return (std::fabs(fCurrentVal - fEndVal) < fTolerance);
   }

   bool IsFinished() override
   {
#if 1
      // we are finished in either of these cases: 
      // 1. user/code canceled us
      // 2. current value is within tolerance of the end value. 
      return (this->ValueIsWithinTolerance() || fCanceled);
#else 
      return (std::fabs(fCurrentVal - fEndVal) < fTolerance) || fCanceled;
#endif
   }
      
protected: 
   float fTolerance;
};


class TimedValue : public AnimatedValue 
{
public:
   TimedValue(float startVal, float endVal, int duration)
   :  AnimatedValue(startVal, endVal)
   ,  fDuration(duration)
   {
      
   }
   
   bool IsFinished() override 
   {
      return fFrameCount >= fDuration;
   }
   
protected: 
   int fDuration;
   
};

}