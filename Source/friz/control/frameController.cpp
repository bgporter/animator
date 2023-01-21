//
// Copyright (c) 2022 Brett g Porter. All Rights Reserved.
//

#include "frameController.h"
#include "animator.h"

namespace friz
{

void FrameController::SetAnimator (Animator* animator_)
{
    animator = animator_;
}

void FrameController::SetFrameRate (int frameRate_)
{
    frameRate = frameRate_;
}

TimeController::TimeController () {}

void TimeController::Start ()
{
    jassert (nullptr != animator);
    startTimerHz (frameRate);
}

void TimeController::Stop ()
{
    stopTimer ();
}

bool TimeController::IsRunning ()
{
    return isTimerRunning ();
}

void TimeController::timerCallback ()
{
    if (animator != nullptr)
        animator->UpdateFrame ();
}

void AsyncController::AdvanceToNextFrame ()
{
    jassert (nullptr != animator);
    animator->UpdateFrame ();
}

} // namespace friz
