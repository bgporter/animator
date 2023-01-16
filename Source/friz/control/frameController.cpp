//
// Copyright (c) 2022 Brett g Porter. All Rights Reserved.
//

#include "frameController.h"
#include "animator.h"

namespace friz
{

void FrameController::setAnimator (Animator* animator_)
{
    animator = animator_;
}

void FrameController::setFrameRate (int frameRate_)
{
    frameRate = frameRate_;
}

TimeController::TimeController () {}

void TimeController::start ()
{
    jassert (nullptr != animator);
    startTimerHz (frameRate);
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
    if (animator != nullptr)
        animator->updateFrame ();
}

void AsyncController::advanceToNextFrame ()
{
    jassert (nullptr != animator);
    animator->updateFrame ();
}

} // namespace friz
