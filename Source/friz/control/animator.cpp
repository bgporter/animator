/*
 * Copyright (c) 2019 Brett g Porter.
 */
#include "animator.h"
#include "frameController.h"
namespace friz
{

Animator::Animator (int frameRate_, std::unique_ptr<FrameController> controller)
: frameRate (frameRate_)
{
    if (controller)
        frameController = std::move (controller);
    else
        frameController = std::make_unique<TimeController> ();

    frameController->SetAnimator (this);
    frameController->SetFrameRate (frameRate);
}

Animator::~Animator ()
{
    frameController->Stop ();
}

void Animator::UpdateFrame ()
{
    int finishedCount = 0;
    juce::ScopedLock lock (mutex);

    for (int i { 0 }; i < animations.size (); ++i)
    {
        auto& animation { animations[i] };
        if (animation.get ())
        {
            if (AnimationType::Status::finished == animation->Update ())
                ++finishedCount;
        }
    }
    if (finishedCount > 0)
        Cleanup ();
}

int Animator::TimeToFrames (float seconds) const
{
    jassert (seconds > 0);

    auto frames = static_cast<int> (0.5f + seconds * frameRate);
    return std::max (frames, 1);
}

bool Animator::AddAnimation (std::unique_ptr<AnimationType> animation)
{
    // In debug builds, verify that the animation has valid AnimatedValue
    // objects before accepting it in the animator.
    jassert (animation->IsReady ());

    {
        juce::ScopedLock lock (mutex);
        animations.push_back (std::move (animation));
    }

    if (!frameController->IsRunning ())
        frameController->Start ();
    return true;
}

bool Animator::CancelAnimation (int id, bool moveToEndPosition)
{
    int cancelCount { 0 };
    juce::ScopedLock lock (mutex);
    for (auto& animation : animations)
    {
        if ((id < 0) || (animation->GetId () == id))
        {
            animation->Cancel (moveToEndPosition);
            ++cancelCount;
        }
    }
    if (cancelCount == 0)
        return false;

    // remove any animations we just canceled.
    Cleanup ();
    return true;
}

bool Animator::CancelAllAnimations (bool moveToEndPosition)
{
    return CancelAnimation (-1, moveToEndPosition);
}

void Animator::Cleanup ()
{
    juce::ScopedLock lock (mutex);
    animations.erase (
        std::remove_if (animations.begin (), animations.end (),
                        [&] (const std::unique_ptr<AnimationType>& c) -> bool
                        { return c->IsFinished (); }),
        animations.end ());

    if (0 == animations.size ())
        frameController->Stop ();
}

AnimationType* Animator::GetAnimation (int id)
{
    juce::ScopedLock lock (mutex);
    for (auto& animation : animations)
    {
        if (id == animation->GetId ())
            return animation.get ();
    }
    return nullptr;
}

int Animator::GetAnimations (int id, std::vector<AnimationType*>& foundAnimations)
{
    int foundCount { 0 };

    juce::ScopedLock lock (mutex);
    for (auto& animation : animations)
    {
        if (id == animation->GetId ())
        {
            foundAnimations.push_back (animation.get ());
            ++foundCount;
        }
    }
    return foundCount;
}

bool Animator::UpdateTarget (int id, int valueIndex, float newTarget)
{
    juce::ScopedLock lock (mutex);

    std::vector<AnimationType*> foundAnimations;

    if (GetAnimations (id, foundAnimations) > 0)
    {
        for (auto* animation : foundAnimations)
        {
            auto* value { animation->GetValue (valueIndex) };
            if (value)
                value->UpdateTarget (newTarget);
        }
        return true;
    }
    return false;
}

#ifdef qRunUnitTests
#include "test/test_Animator.cpp"
#endif

} // namespace friz
