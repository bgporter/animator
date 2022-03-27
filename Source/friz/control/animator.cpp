/*
 * Copyright (c) 2019 Brett g Porter.
 */
#include "animator.h"
#include "frameController.h"
namespace friz
{

Animator::Animator (int frameRate, std::unique_ptr<FrameController> controller)
: fFrameRate (frameRate)
{
    if (controller)
        fFrameController = std::move (controller);
    else
        fFrameController = std::make_unique<TimeController> ();

    fFrameController->setAnimator (this);
    fFrameController->setFrameRate (frameRate);
}

Animator::~Animator ()
{
    fFrameController->stop ();
}

void Animator::updateFrame ()
{
    int finishedCount = 0;
    int updated       = 0;
    juce::ScopedLock lock (fMutex);

    for (int i { 0 }; i < fAnimations.size (); ++i)
    {
        auto& animation { fAnimations[i] };
        if (animation.get ())
        {
            finishedCount += animation->update ();
            ++updated;
        }
    }
    if (finishedCount > 0)
    {
        cleanup ();
    }
}

int Animator::timeToFrames (float seconds) const
{
    jassert (seconds > 0);

    auto frames = static_cast<int> (0.5f + seconds * fFrameRate);
    return std::max (frames, 1);
}

bool Animator::addAnimation (std::unique_ptr<AnimationType> animation)
{
    // In debug builds, verify that the animation has valid AnimatedValue
    // objects before accepting it in the animator.
    jassert (animation->isReady ());

    {
        juce::ScopedLock lock (fMutex);
        fAnimations.push_back (std::move (animation));
    }

    if (!fFrameController->isRunning ())
    {
        fFrameController->start ();
    }
    return true;
}

bool Animator::cancelAnimation (int id, bool moveToEndPosition)
{
    int cancelCount { 0 };
    juce::ScopedLock lock (fMutex);
    for (auto& animation : fAnimations)
    {
        if ((id < 0) || (animation->getId () == id))
        {
            animation->cancel (moveToEndPosition);
            ++cancelCount;
        }
    }
    if (cancelCount > 0)
    {
        // remove any animations we just canceled.
        cleanup ();
        return true;
    }

    return false;
}

bool Animator::cancelAllAnimations (bool moveToEndPosition)
{
    return cancelAnimation (-1, moveToEndPosition);
}

void Animator::cleanup ()
{
    juce::ScopedLock lock (fMutex);
    fAnimations.erase (
        std::remove_if (fAnimations.begin (), fAnimations.end (),
                        [&] (const std::unique_ptr<AnimationType>& c) -> bool
                        { return c->isFinished (); }),
        fAnimations.end ());

    if (0 == fAnimations.size ())
    {
        fFrameController->stop ();
    }
}

AnimationType* Animator::getAnimation (int id)
{
    juce::ScopedLock lock (fMutex);
    for (auto& animation : fAnimations)
    {
        if (id == animation->getId ())
        {
            return animation.get ();
        }
    }
    return nullptr;
}

int Animator::getAnimations (int id, std::vector<AnimationType*>& animations)
{
    int foundCount { 0 };

    juce::ScopedLock lock (fMutex);
    for (auto& animation : fAnimations)
    {
        if (id == animation->getId ())
        {
            animations.push_back (animation.get ());
            ++foundCount;
        }
    }
    return foundCount;
}

bool Animator::updateTarget (int id, int valueIndex, float newTarget)
{
    juce::ScopedLock lock (fMutex);

    std::vector<AnimationType*> animations;

    if (getAnimations (id, animations) > 0)
    {
        for (auto* animation : animations)
        {
            auto* value { animation->getValue (valueIndex) };
            if (value)
            {
                value->updateTarget (newTarget);
            }
        }
        return true;
    }
    return false;
}

#ifdef qRunUnitTests
#include "test/test_Animator.cpp"
#endif

} // namespace friz
