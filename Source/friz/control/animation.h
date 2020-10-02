/*
 * Copyright (c) 2019 Brett g Porter. 
 */
#pragma once

// #include "../animatorApp.h"
#include <array>

#include "../curves/animatedValue.h"

namespace friz 
{

/**
 * @class AnimationType 
 *
 * @brief Abstract base class; all the real action happens in the derived 
 *        templated `Animation` class, below. 
 */

class AnimationType 
{
public:
   
   enum 
   {
      kProcessing = 0,
      kFinished
   };
   
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
   
   /**
    * Set a number of frames to delay before starting to execute this animation. 
    * @param delay # of delay frames. 
    */
   void SetDelay(int delay) 
   {
      if (delay >= 0)
      {
         fDelay = delay;
      }
   }
   
   /**
    * Before generating values, see if we should be delaying. 
    * @return False to wait before generating effect values. 
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
   
   virtual bool IsReady() const = 0;
   
private:
   /// optional ID value for this animation. 
   int fId;
   
   /// an optional pre-delay before beginning to execute the effect. 
   int fDelay; 
   
};


/**
 * @class Animation 
 *
 * @brief This class owns a number of `AnimatedValue` objects. On each animation 
 * frame it gets the next calculated value from each of the value objects and 
 * passes those values to its `OnUpdate` handler. When all of the values in the 
 * animation have reached their end states, calls the `OnCompletion` handler. 
 *
 * Once this animation is complete, the `Animator` object that owns it will 
 * garbage collect it. 
 */


template <std::size_t valueCount>
class Animation  : public AnimationType
{
public:
   
   using ValueList = std::array<float, valueCount>;
   using SourceList = std::array<std::unique_ptr<AnimatedValue>, valueCount>;
   using UpdateFn = std::function<void(int, const ValueList&)>;
   using CompletionFn = std::function<void(int)>;
   

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
   
   Animation(SourceList&& sources, int id=0)
   :  AnimationType(id)
   ,  fFinished(false)
   ,  fSources(std::move(sources))
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
         fSources[index] = std::move(value);
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
         return kProcessing;
      }
      
      for (int i = 0; i < valueCount; ++i)
      {
         auto& val = fSources[i];
         jassert(val);
         if (val)
         {
            values[i] = val->GetNextValue();
            completeCount += (val->IsFinished())  ? 1 : 0;
         }
      }
      
      if (fUpdateFn)
      {
         fUpdateFn(this->GetId(), values);
      }
      
      
      if (completeCount == valueCount)
      {
         if (fCompleteFn)
         {
            fCompleteFn(this->GetId());
         }
         fFinished = true;
         return kFinished;
      }
      
      return kProcessing;
   }
   
   void Cancel(bool moveToEndPosition) override
   {
      for (int i = 0; i < valueCount; ++i)
      {
         auto& val = fSources[i];
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
            fCompleteFn(this->GetId());
         }
      }
      fFinished = true;
   }
   
   bool IsFinished() override 
   {
       return fFinished;
   }
   
   bool IsReady() const override
   {
      for (auto& src: fSources)
      {
         if (nullptr == src.get())
         {
            return false;
         }
      }
      return true;
   }
protected: 
   /// function to call on each frame. Pass in std::array of new values, 
   /// return true if all is okay, false to cancel this animation. 
   UpdateFn fUpdateFn;

   /// function to call when the animation is completed or canceled. 
   CompletionFn fCompleteFn;
   
private:
   /// is this animation complete? 
   bool fFinished; 
   
   /// The array of animated value objects. 
   // std::array<std::unique_ptr<AnimatedValue>, valueCount> fSources;
   SourceList fSources;

};


} // namespace friz
