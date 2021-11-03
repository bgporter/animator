/*
 * Copyright (c) 2019 Brett g Porter. 
 */
#include "animator.h"


namespace friz 
{
   
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
   int finishedCount = 0;
   int updated = 0;

   juce::ScopedLock lock(fMutex);
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



int Animator::TimeToFrames(float seconds)
{
   jassert(seconds > 0);
   
   auto frames = static_cast<int>(0.5f + seconds * fFrameRate);
   return std::max(frames, 1);
}

bool Animator::AddAnimation(std::unique_ptr<AnimationType> animation)
{
   
   char address[50];
   sprintf(address, "%p", (void*) animation.get());
   
   
   // DBG("AddAnimation # " << animation->GetId() << " @ " << juce::String(address)); 
   
   // In debug builds, verify that the animation has valid AnimatedValue 
   // objects before accepting it in the animator. 
   jassert(animation->IsReady());
   
   {
       juce::ScopedLock lock(fMutex);
       fAnimations.push_back(std::move(animation));
   }
   
   if (! this->isTimerRunning())
   {
        // DBG("startng timer");
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

    {
        juce::ScopedLock lock(fMutex);
        fAnimations.erase(std::remove_if(fAnimations.begin(), fAnimations.end(), 
            [&] (const std::unique_ptr<AnimationType>& c) -> bool 
            {
                return c->IsFinished();
            }
            ), fAnimations.end());
    }
    
    if (0 == fAnimations.size())
    {
        // DBG("stopping timer");
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

bool Animator::UpdateTarget(int id, float newTarget)
{
    juce::ScopedLock lock(fMutex);
    std::vector<AnimationType*> animations;
    this->GetAnimations(id, animations);
    int updateCount{0};
    for (auto* animation : animations)
    {
        animation->UpdateTarget(newTarget);
        ++updateCount;
    }
    return (updateCount > 0);
}



#ifdef qRunUnitTests
#include "test/test_Animator.cpp"
#endif


} // namespace friz
