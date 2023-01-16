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

    frameController->setAnimator (this);
    frameController->setFrameRate (frameRate);
}

Animator::~Animator ()
{
    frameController->stop ();
}

void Animator::updateFrame ()
{
    int finishedCount = 0;
    juce::ScopedLock lock (mutex);

    for (int i { 0 }; i < animations.size (); ++i)
    {
        auto& animation { animations[i] };
        if (animation.get ())
        {
            if (AnimationType::Status::finished == animation->update ())
                ++finishedCount;
        }
    }
    if (finishedCount > 0)
        cleanup ();
}

int Animator::timeToFrames (float seconds) const
{
    jassert (seconds > 0);

    auto frames = static_cast<int> (0.5f + seconds * frameRate);
    return std::max (frames, 1);
}

bool Animator::addAnimation (std::unique_ptr<AnimationType> animation)
{
    // In debug builds, verify that the animation has valid AnimatedValue
    // objects before accepting it in the animator.
    jassert (animation->isReady ());

    {
        juce::ScopedLock lock (mutex);
        animations.push_back (std::move (animation));
    }

    if (!frameController->isRunning ())
        frameController->start ();
    return true;
}

bool Animator::cancelAnimation (int id, bool moveToEndPosition)
{
    int cancelCount { 0 };
    juce::ScopedLock lock (mutex);
    for (auto& animation : animations)
    {
        if ((id < 0) || (animation->getId () == id))
        {
            animation->cancel (moveToEndPosition);
            ++cancelCount;
        }
    }
    if (cancelCount == 0)
        return false;

    // remove any animations we just canceled.
    cleanup ();
    return true;
}

bool Animator::cancelAllAnimations (bool moveToEndPosition)
{
    return cancelAnimation (-1, moveToEndPosition);
}

void Animator::cleanup ()
{
    juce::ScopedLock lock (mutex);
    animations.erase (
        std::remove_if (animations.begin (), animations.end (),
                        [&] (const std::unique_ptr<AnimationType>& c) -> bool
                        { return c->isFinished (); }),
        animations.end ());

    if (0 == animations.size ())
        frameController->stop ();
}

AnimationType* Animator::getAnimation (int id)
{
    juce::ScopedLock lock (mutex);
    for (auto& animation : animations)
    {
        if (id == animation->getId ())
            return animation.get ();
    }
    return nullptr;
}

int Animator::getAnimations (int id, std::vector<AnimationType*>& foundAnimations)
{
    int foundCount { 0 };

    juce::ScopedLock lock (mutex);
    for (auto& animation : animations)
    {
        if (id == animation->getId ())
        {
            foundAnimations.push_back (animation.get ());
            ++foundCount;
        }
    }
    return foundCount;
}

bool Animator::updateTarget (int id, int valueIndex, float newTarget)
{
    juce::ScopedLock lock (mutex);

    std::vector<AnimationType*> animations;

    if (getAnimations (id, animations) > 0)
    {
        for (auto* animation : animations)
        {
            auto* value { animation->getValue (valueIndex) };
            if (value)
                value->updateTarget (newTarget);
        }
        return true;
    }
    return false;
}

#ifdef qRunUnitTests
#include "test/test_Animator.cpp"
#endif

} // namespace friz
