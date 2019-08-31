/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animation.h"

template <int valueCount>
class Sequence : public AnimationType
{
public:
   Sequence(int id=0)
   :  AnimationType(0)
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
         
         return this->IsFinished() ? 1 : 0;
      }
      
      return 1;
   }
   
   void Cancel(bool moveToEndPosition) override
   {
      if (moveToEndPosition)
      {
         auto last = this->GetEffect(fSequence.size()-1); 
         if (last)
         {
            last->Cancel(moveToEndPosition);
         }
      }
      
   }
   
   
   void AddAnimation(std::unique_ptr<Animation<valueCount>> effect)
   {
      fSequence.push_back(std::move(effect));
   }
   
   
private:
   
   AnimationType* GetEffect(int index)
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
