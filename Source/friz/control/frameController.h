//
// Copyright (c) 2022 Brett g Porter. All Rights Reserved.
//

#pragma once

namespace friz
{

class Animator;

class FrameController
{
public:
    FrameController ()                                  = default;
    virtual ~FrameController ()                         = default;
    FrameController (const FrameController&)            = delete;
    FrameController& operator= (const FrameController&) = delete;
    FrameController (FrameController&&)                 = delete;
    FrameController& operator= (FrameController&&)      = delete;

    /** Point back at the Animator object that we're going to control.
     */
    void SetAnimator (Animator* animator);

    /**
     * Set our frame rate to match that used in the animation we're controlling.
     * @param frameRate
     */
    void SetFrameRate (int frameRate);

    /**
     * @brief Called whenever we need to start timer callbacks flowing.
     */
    virtual void Start () = 0;

    /**
     * @brief  Called whenever there are no more animations that need to
     * be updated.
     */
    virtual void Stop () = 0;

    /**
     * @brief Test to see if the timer is currently running.
     *
     */
    virtual bool IsRunning () = 0;

protected:
    Animator* animator { nullptr };
    int frameRate { 30 };
};

/**
 * @class TimeController
 *
 * A FrameController that updates animation frames at the current frame rate.
 */
class TimeController : public FrameController,
                       public juce::Timer
{
public:
    TimeController ();

    void Start () override;

    void Stop () override;

    bool IsRunning () override;

private:
    void timerCallback () override;
};

class AsyncController : public FrameController
{
public:
    AsyncController () {};

    void Start () override { started = true; }

    void Stop () override { started = false; }

    bool IsRunning () override { return started; }

    /**
     * @brief Call this from whatever logic you're using to drive the
     * animation system. This tells the animator to advance to its next frame,
     * which will only actually have an effect if there are any animations running
     * at the current time.
     *
     */
    void AdvanceToNextFrame ();

private:
    bool started { false };
};

} // namespace friz
