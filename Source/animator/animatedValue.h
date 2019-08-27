/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "../animatorApp.h"


class AnimatedValue 
{
public:
   /**
    * Base class init for the animated value classes. 
    * @param startVal  Initial Value 
    * @param endVal    Target/end value. 
    * @param tolerance Toleerance for when we've reached the target value.
    */
   AnimatedValue(float startVal, float endVal, float tolerance)
   :  fStartVal(startVal)
   ,  fEndVal(endVal)
   ,  fTolerance(tolerance)
   ,  fCurrentVal(startVal)
   ,  fLastTime(-1)
   {
      
   };
   
   virtual ~AnimatedValue()
   {
      
   };
   
   /**
    * Derived classes should do whatever is needed to generate and 
    * return the next value. 
    * @param  msTime current time in ms. 
    * @return        next value (or last value if we're finished)
    */
   float GetNextValue(int msTime)
   {
      if (-1 == fLastTime)
      {
         fCurrentVal = fStartVal;
         fLastTime = msTime;
      }
      else 
      {
         fCurrentVal = this->GenerateNextValue(msTime - fLastTime);
         fLastTime = msTime;
      }
      return fCurrentVal;
   }
   
   /**
    * Have we reached the end of this animation sequence? By default, 
    * we're done when the current value is within `fTolerance` of the endValue.
    * @return [description]
    */
   virtual bool IsFinished()
   {
      return (std::fabs(fCurrentVal - fEndVal) < fTolerance);
   }
   
   /**
    * Reset the object so it can run again using the same start/end values.
    */
   void Reset()
   {
      fLastTime = -1;
      this->DoReset();
   }


private:
   
   /**
    * Implemented in base classes to generate the next value in the sequence. 
    * @param  time ms since the last time we were called. 
    * @return      next value.
    */
   virtual float GenerateNextValue(int deltaTime) = 0;
   
   /**
    * Override in derived classes to perform any additional reset of calculations. 
    */
   virtual void DoReset()
   {
      
   }


protected: 
   float fStartVal;
   float fEndVal;
   float fTolerance;
   float fCurrentVal;
   
   int fLastTime;
   
private:
   
};
