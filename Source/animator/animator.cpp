/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "animator.h"


Animator::Animator(int frameRate)
:  fFrameRate(frameRate)
{
   
}

Animator::~Animator()
{
   this->stopTimer();
}

void Animator::timerCallback() 
{
   
}

bool Animator::AddAnimation(std::unique_ptr<AnimationType> animation)
{
   fAnimations.push_back(std::move(animation));
   return true;
   
}

bool Animator::CancelAnimation(int id, bool moveToEndPosition)
{
   int cancelCount = 0;
   for (auto& animation: fAnimations)
   {
      if (animation->GetId() == id)
      {
         animation->Cancel(moveToEndPosition);
         ++cancelCount;
      }
   }
   if (cancelCount > 0)
   {
      // remove any animations we just canceled.
      this->Cleanup();
   }
   
   return (cancelCount > 0);
}

bool Animator::CancelAllAnimations(bool moveToEndPosition)
{
   int cancelCount{0};
   
   for (auto& animation: fAnimations)
   {
      animation->Cancel(moveToEndPosition);
      ++cancelCount;
   }
   if (cancelCount > 0)
   {
      // remove any animations we just canceled.
      this->Cleanup();
   }
   
   return (cancelCount > 0);
   
}

void Animator::Cleanup()
{
   fAnimations.erase(std::remove_if(fAnimations.begin(), fAnimations.end(), 
      [] (const std::unique_ptr<AnimationType>& c) -> bool {
         return c->IsFinished();
   }));
}


