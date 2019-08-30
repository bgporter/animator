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
   
   if (! this->isTimerRunning())
   {
      this->startTimerHz(fFrameRate);
   }
   return true;
   
}

bool Animator::CancelAnimation(int id, bool moveToEndPosition)
{
   int cancelCount = 0;
   for (auto& animation: fAnimations)
   {
      if ((id < 0) || (animation->GetId() == id))
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
   return this->CancelAnimation(-1, moveToEndPosition);
}

void Animator::Cleanup()
{
   fAnimations.erase(std::remove_if(fAnimations.begin(), fAnimations.end(), 
      [] (const std::unique_ptr<AnimationType>& c) -> bool {
         return c->IsFinished();
   }));
   
   if (0 == fAnimations.size())
   {
      this->stopTimer();
   }
}


