/*
 * Copyright (c) 2019 Brett g Porter.
 */
#include "animator.h"
#include "controller.h"
namespace friz
{

Animator::Animator (std::unique_ptr<Controller> controller_)
{
    if (controller_ != nullptr)
        setController (std::move (controller_));
    else
        setController (std::make_unique<TimeController> ());

    controller->setAnimator (this);
}

Animator::~Animator ()
{
    controller->stop ();
}

void Animator::setController (std::unique_ptr<Controller> controller_)
{
    controller = std::move (controller_);
}

Controller* Animator::getController () const
{
    return controller.get ();
}

bool Animator::setFrameRate (int rateInHz)
{
    if (controller == nullptr)
    {
        jassertfalse;
        return false;
    }

    return controller->setFrameRate (rateInHz);
}

float Animator::getFrameRate () const
{
    if (controller == nullptr)
    {
        jassertfalse;
        return -1.f;
    }

    return controller->getFrameRate ();
}

void Animator::gotoTime (juce::int64 timeInMs)
{
    int finishedCount { 0 };
    juce::ScopedLock lock { mutex };

    // for (auto& animation : animations)
    for (int i { 0 }; i < animations.size (); ++i)
    {
        auto& animation { animations[i] };
        if (animation.get () != nullptr)
        {
            if (AnimationType::Status::finished == animation->gotoTime (timeInMs))
                ++finishedCount;
        }
    }
    if (finishedCount > 0)
        cleanup ();
}

bool Animator::addAnimation (std::unique_ptr<AnimationType> animation)
{
    // In debug builds, verify that the animation has valid AnimatedValue
    // objects before accepting it in the animator.
    if (!animation->isReady ())
    {
        jassertfalse;
        return false;
    }

    juce::ScopedLock lock (mutex);
    DBG ("ADDING ANIMATION");
    animations.push_back (std::move (animation));

    if (!controller->isRunning ())
    {
        DBG ("STARTING CONTROLLER ");
        controller->start ();
    }

    return true;
}

bool Animator::cancelAnimation (int id, bool moveToEndPosition)
{
    int cancelCount { 0 };
    juce::ScopedLock lock (mutex);
    DBG ("CANCEL ANIMATION ID " << id);
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
    DBG ("CLEANUP: " << animations.size () << " animations @ start");
    animations.erase (
        std::remove_if (animations.begin (), animations.end (),
                        [&] (const std::unique_ptr<AnimationType>& c) -> bool
                        { return c->isFinished (); }),
        animations.end ());

    DBG ("CLEANUP: " << animations.size () << " animations @ end");
    if (0 == animations.size ())
    {
        DBG (" STOPPING CONTROLLER ");
        controller->stop ();
    }
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

    std::vector<AnimationType*> foundAnimations;

    if (getAnimations (id, foundAnimations) > 0)
    {
        for (auto* animation : foundAnimations)
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
