/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#pragma once

#include "../animatorApp.h"
#include <array>

#include "animatedValue.h"

class AnimationType 
{
public:
   
   AnimationType(int id)
   :  fId{id}
   ,  fDelay{0}
   {
      // only allow positive animation IDs. 
      jassert(id >= 0);
      
   }
   
   virtual ~AnimationType() = default;
   /**
    * @return ID value for this Animation. 
    */
   int GetId() const 
   {
      return fId;
   }
   
   void SetDelay(int delay) 
   {
      if (delay >= 0)
      {
         fDelay = delay;
      }
   }
   
   /**
    * Before generating values, see if we should be delaying. 
    * @return True to wait before generating effect values. 
    */
   bool DelayElapsed() 
   {
      if (0 == fDelay)
      {
         return true;
      }
      
      --fDelay;
      return false;
      
   }
   
   virtual int Update() = 0;
   
   virtual void Cancel(bool moveToEndPosition) = 0;
   
   virtual bool IsFinished() = 0;
   
private:
   /// optional ID value for this animation. 
   int fId;
   
   /// an optional pre-delay before beginning to execute the effect. 
   int fDelay; 
   
};


template <std::size_t valueCount>
class Animation  : public AnimationType
{
public:
   
   using ValueList = std::array<float, valueCount>;
   using UpdateFn = std::function<void(const ValueList&)>;
   using CompletionFn = std::function<void(void)>;
   

   /**
    * Create an animation object that can be populated with changing 
    * values and functions to call at important points (each frame of animation, 
    * sequence completion)
    * 
    * @param id Optional identifier, use as you wish. We don't enforce uniqueness, 
    *           for example. Must be >= 0. 
    */
   Animation(int id=0)
   :  AnimationType(id)
   ,  fFinished{false}
   {
      
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
   void OnUpdate(UpdateFn update)
   {
      fUpdateFn = update;
   }
   
   /**
    * Set the (optional) function that will be called once when this 
    * animation is complete. 
    * @param complete CompletionFn function. 
    */
   void OnCompletion(CompletionFn complete)
   {
      fCompleteFn = complete;
   }
   
   /**
    * Calculate the next value from each of our animated values, passing them 
    * to our UpdateFn function. 
    * @return        Zero if we have more data in the future, 1 if we're done. 
    */
   int Update() override
   {
      ValueList values;
      int completeCount{0};
      
      if (! this->DelayElapsed())
      {
         return 0;
      }
      
      for (int i = 0; i < valueCount; ++i)
      {
         auto& val = fValues[i];
         jassert(val);
         if (val)
         {
            values[i] = val->GetNextValue();
            completeCount += (val->IsFinished())  ? 1 : 0;
         }
      }
      
      if (fUpdateFn)
      {
         fUpdateFn(values);
      }
      
      
      if (completeCount == valueCount)
      {
         if (fCompleteFn)
         {
            fCompleteFn();
         }
         fFinished = true;
         return 1;
      }
      
      return 0;
   }
   
   void Cancel(bool moveToEndPosition) override
   {
      for (int i = 0; i < valueCount; ++i)
      {
         auto& val = fValues[i];
         jassert(val);
         if (val)
         {
            val->Cancel(moveToEndPosition);
         }
      }
      
      if (moveToEndPosition)
      {
         // send out one more value update message sending the end positions;
         this->Update();
      }
      else 
      {
         // ...just notify that the effect is complete. 
         if (fCompleteFn)
         {
            fCompleteFn();
         }
      }
      fFinished = true;
   }
   
   bool IsFinished() override 
   {
       return fFinished;
   }
   
private:
   
   
   /// is this animation complete? 
   bool fFinished; 
   
   /// The array of animated value objects. 
   std::array<std::unique_ptr<AnimatedValue>, valueCount> fValues;
   
   /// function to call on each frame. Pass in std::array of new values, 
   /// return true if all is okay, false to cancel this animation. 
   UpdateFn fUpdateFn;
   
   /// function to call when the animation is completed or canceled. 
   CompletionFn fCompleteFn;
   
   
   
};
