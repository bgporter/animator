/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animation.h"

/**
 * @class Sequence 
 *
 * @brief An Animation class that can hold multiple Animation objects and 
 *        execute them in sequence. An easy way to create complex effects 
 *        from promotove movements. 
 */


template <int valueCount>
class Sequence : public Animation<valueCount>
{
public:
   Sequence(int id=0)
   :  Animation<valueCount>(id)
   ,  fCurrentEffect{0}
   {
      
   }
   
   bool IsFinished() override 
   {
      return (fCurrentEffect >= fSequence.size());
   }
   
   int Update() override 
   {
      auto effect = this->GetEffect(fCurrentEffect);
      if (effect)
      {
         if (effect->Update())
         {
            ++fCurrentEffect;
         }
         
         return this->IsFinished() ? AnimationType::kFinished : AnimationType::kProcessing;
      }
      
      return AnimationType::kFinished;
   }
   
   void Cancel(bool moveToEndPosition) override
   {
      if (moveToEndPosition)
      {
         auto lastIndex = static_cast<int>(fSequence.size() - 1);
         auto lastEffect = this->GetEffect(lastIndex); 
         if (lastEffect)
         {
            lastEffect->Cancel(moveToEndPosition);
         }
      }
      
   }
   
   
   void AddAnimation(std::unique_ptr<Animation<valueCount>> effect)
   {
      // We need to make each of the effects notify us on update or completion, 
      // so that we can pass those along to whoever passed in a single 
      // lambda to us. 
      effect->OnUpdate([=] (int id, const typename Animation<valueCount>::ValueList& val){
         if (this->fUpdateFn)
         {
            this->fUpdateFn(this->GetId(), val);
         }
         
      });
      
      effect->OnCompletion([=] (int id){
         // Each effect in the sequence will notify us, but we only pass 
         // along the final one. 
         if ((fCurrentEffect == fSequence.size() - 1) && this->fCompleteFn)
         {
            this->fCompleteFn(this->GetId());
         }
      });
      
      fSequence.push_back(std::move(effect));
      
   }
   
   
   
private:
   /**
    * Get a pointer to one of our effects by its index. 
    * @param  index 0..size-1
    * @return       nullptr if index is out of range. 
    */
   Animation<valueCount>* GetEffect(int index)
   {
      if (isPositiveAndBelow(index, fSequence.size()))
      {
         return fSequence[index].get();
      }
      jassert(false);
      return nullptr;
   }
   

   
private:
   
   std::vector<std::unique_ptr<Animation<valueCount> >> fSequence;
   
   int fCurrentEffect;
};
