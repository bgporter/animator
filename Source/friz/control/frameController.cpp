//
// Copyright (c) 2022 Brett g Porter. All Rights Reserved.
//

#include "frameController.h"
#include "animator.h"

namespace friz
{
FrameController::FrameController (int frameRate)
: fFrameRate (frameRate)
{
    jassert (frameRate > 0);
}

void FrameController::setAnimator (Animator* animator)
{
    fAnimator = animator;
}

void FrameController::setFrameRate (int frameRate)
{
    fFrameRate = frameRate;
}

TimeController::TimeController (int frameRate)
: FrameController (frameRate)
{
}

void TimeController::start ()
{
    jassert (nullptr != fAnimator);
    startTimerHz (fFrameRate);
}

void TimeController::stop ()
{
    stopTimer ();
}

bool TimeController::isRunning ()
{
    return isTimerRunning ();
}

void TimeController::timerCallback ()
{
    if (fAnimator)
        fAnimator->updateFrame ();
}

} // namespace friz