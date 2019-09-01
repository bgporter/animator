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
   const ScopedLock sl(fMutex);
   int finishedCount = 0;
   int updated = 0;
   // for (auto& animation : fAnimations)
   for (int i = 0; i < fAnimations.size(); ++i)
   {
      auto animation = fAnimations[i].get();
      assert(animation);
      finishedCount += animation->Update();
      ++updated;
   }
   
   if (finishedCount > 0)
   {
      this->Cleanup();
   }
}

bool Animator::AddAnimation(std::unique_ptr<AnimationType> animation)
{
   const ScopedLock sl(fMutex);
   
   char address[50];
   sprintf(address, "%p", (void*) animation.get());
   
   
   DBG("AddAnimation # " << animation->GetId() << " @ " << String(address)); 
   fAnimations.push_back(std::move(animation));
   
   if (! this->isTimerRunning())
   {
      DBG("startng timer");
      this->startTimerHz(fFrameRate);
   }
   return true;
   
}

bool Animator::CancelAnimation(int id, bool moveToEndPosition)
{
   const ScopedLock sl(fMutex);
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
      [&] (const std::unique_ptr<AnimationType>& c) -> bool 
      {
         return c->IsFinished();
      }
      ), fAnimations.end());
   
   if (0 == fAnimations.size())
   {
      DBG("stopping timer");
      this->stopTimer();
   }
}


AnimationType* Animator::GetAnimation(int id)
{
   for (auto& animation: fAnimations)
   {
      if (id == animation->GetId())
      {
         return animation.get();
      }
   }
   return nullptr;
}


int Animator::GetAnimations(int id, std::vector<AnimationType*>& animations)
{
   int foundCount{0};
   
   for (auto& animation: fAnimations)
   {
      if (id == animation->GetId())
      {
         animations.push_back(animation.get());
         ++foundCount;
      }
   }
   return foundCount;
}

#ifdef qRunUnitTests
#include "test/test_Animator.cpp"
#endif
