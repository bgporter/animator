/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#pragma once

#include "../animatorApp.h"
#include <array>

#include "animatedValue.h"


template <std::size_t valueCount>
class AnimationController 
{
public:
   
   using ValueList = std::array<float, valueCount>;
   using UpdateFn = std::function<bool(const ValueList&)>;
   using CompletionFn = std::function<void(void)>;
   

   /**
    * Create an animationController object that can be populated with changing 
    * values and functions to call at important points (each frame of animation, 
    * sequence completion)
    * 
    * @param id Optional identifier, use as you wish. We don't enforce uniqueness, 
    *           for example. 
    */
   AnimationController(int id=0)
   :  fId{0}
   {
      
   }
   
   /**
    * @return ID value for this controller. 
    */
   int GetId() const 
   {
      return fId;
   }
   
   /**
    * Set the AnimatedValue object to use for one of this animation's slots. 
    * @param  index Value index, 0..valueCount-1
    * @param  value AnimatedValue object to generate data. 
    * @return       true on success. 
    */
   bool SetValue(size_t index, std::unique_ptr<AnimatedValue> value)
   {
      if (index < valueCount)
      {
         fValues[index] = std::move(value);
         return true;
      }
      jassert(false);
      return false;
   }
   
   /**
    * Set the function that will be called with an array of animation values 
    * once per frame. 
    * @param update UpdateFn function. 
    */
   void SetUpdateFunction(UpdateFn update)
   {
      fUpdateFn = update;
   }
   
   /**
    * Set the (optional) function that will be called once when this 
    * animation is complete. 
    * @param complete CompletionFn function. 
    */
   void SetCompletionFunction(CompletionFn complete)
   {
      fCompleteFn = complete;
   }
   
   /**
    * Calculate the next value from each of our animated values, passing them 
    * to our UpdateFn function. 
    * @return        Zero if we have more data in the future, 1 if we're done. 
    */
   int Update()
   {
      ValueList values;
      int completeCount{0};
      
      for (int i = 0; i < valueCount; ++i)
      {
         auto& ptr = fValues[i];
         if (ptr)
         {
            values[i] = ptr->GetNextValue();
            completeCount += (ptr->IsFinished())  ? 1 : 0;
         }
         else 
         {
            jassert(false);
         }
      }
      
      if (fUpdateFn)
      {
         bool ok = fUpdateFn(values);
      }
      
      
      if (completeCount == valueCount)
      {
         if (fCompleteFn)
         {
            fCompleteFn();
         }
         return 1;
      }
      
      return 0;
   }
   
   
   
private:
   
   /// optional ID value for this animation. 
   int fId;
   
   /// The array of animated value objects. 
   std::array<std::unique_ptr<AnimatedValue>, valueCount> fValues;
   
   /// function to call on each frame. Pass in std::array of new values, 
   /// return true if all is okay, false to cancel this animation. 
   UpdateFn fUpdateFn;
   
   /// function to call when the animation is completed or canceled. 
   CompletionFn fCompleteFn;
   
   
   
};
