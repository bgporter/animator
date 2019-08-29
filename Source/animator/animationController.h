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
   using UpdateFn = std::function<bool(ValueList)>;
   using CompletionFn = std::function<void(void)>;
   
   
   AnimationController()
   {
      
   }
   
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
   
   void SetUpdateFunction(UpdateFn update)
   {
      fUpdateFn = update;
   }
   
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
      std::array<float, valueCount> values;
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
   
   std::array<std::unique_ptr<AnimatedValue>, valueCount> fValues;
   
   /// function to call on each frame. Pass in std::array of new values, 
   /// return true if all is okay, false to cancel this animation. 
   UpdateFn fUpdateFn;
   
   /// function to call when the animation is completed or canceled. 
   CompletionFn fCompleteFn;
   
   
   
};
