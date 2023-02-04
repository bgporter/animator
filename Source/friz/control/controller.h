// Copyright (c) 2023 Brett g Porter

#pragma once

namespace friz
{
class Animator;

class Controller
{
public:
    Controller ()                             = default;
    virtual ~Controller ()                    = default;
    Controller (const Controller&)            = delete;
    Controller& operator= (const Controller&) = delete;
    Controller (Controller&&)                 = delete;
    Controller& operator= (Controller&&)      = delete;

    /** Point back at the Animator object that we're going to control.
     */
    void setAnimator (Animator* animator_) { animator = animator_; }

    /**
     * Set our frame rate to match that used in the animation we're controlling.
     * @param frameRate
     */
    virtual bool setFrameRate (int frameRate) { return false; }

    /**
     * @brief report the current frame rate, whether what was requested or the actual
     * measured rate.
     *
     * @return float
     */
    virtual float getFrameRate () const { return 0.f; }

    /**
     * @brief Called whenever we need to start timer callbacks flowing.
     */
    virtual void start () = 0;

    /**
     * @brief  Called whenever there are no more animations that need to
     * be updated.
     */
    virtual void stop () = 0;

    /**
     * @brief Test to see if the timer is currently running.
     *
     */
    virtual bool isRunning () = 0;

    /**
     * @brief Calculate the current time in milliseconds since some event,
     * probably system start. Probably not accurate enough for e.g. musical purposes
     * but more than accurate enough for visual animations.
     *
     * @return int64 millisecond value.
     */
    static juce::int64 getCurrentTime ();

protected:
    /// @brief  the animator object that owns us.
    Animator* animator;
};

class TimeController : public Controller,
                       public juce::Timer
{
public:
    bool setFrameRate (int frameRate_) override
    {
        jassert (frameRate_ > 0);
        frameRate = frameRate_;
        return true;
    }

    /**
     * @brief report the current frame rate, whether what was requested or the actual
     * measured rate.
     *
     * @return float
     */
    float getFrameRate () const override { return frameRate; }

    /**
     * @brief Called whenever we need to start timer callbacks flowing.
     */
    void start () override
    {
        jassert (animator != nullptr);
        startTimerHz (frameRate);
    }

    /**
     * @brief  Called whenever there are no more animations that need to
     * be updated.
     */
    void stop () override { stopTimer (); }

    /**
     * @brief Test to see if the timer is currently running.
     *
     */
    bool isRunning () override { return isTimerRunning (); }

private:
    void timerCallback () override { animator->gotoTime (getCurrentTime ()); }

private:
    /// @brief Approx. frames/sec
    int frameRate { 30 };
};

} // namespace friz
