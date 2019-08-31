/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animation.h"

template <int valueCount>
class Sequence : public Animation<valueCount>
{
public:
   Sequence(int id=0)
   :  Animation<valueCount>(0)
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
       effect->OnUpdate([=] (const typename Animation<valueCount>::ValueList& val){
         if (this->fUpdateFn)
         {
            this->fUpdateFn(val);
         }
         
      });
      
      fSequence.push_back(std::move(effect));
      
   }
   
   
   
private:
   
   Animation<valueCount>* GetEffect(int index)
   {
      if (index < fSequence.size())
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
