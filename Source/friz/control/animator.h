/*
    Copyright (c) 2019-2023 Brett g Porter

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

// #include "../animatorApp.h"

#include "../curves/constant.h"
#include "../curves/easing.h"
#include "../curves/linear.h"
#include "../curves/spring.h"
#include "animation.h"

namespace friz
{

class Controller;

/**
 * @class Animator
 * @brief A class that can own `Animation` objects and execute them at a regular
 *        interval.
 *
 * Since the timer that drives the animations only executes if there is currently
 * an animation happening, there's no overhead when the animator is idle.
 */
class Animator
{
public:
    /**
     * @brief Construct a new Animator object. If not configured otherwise, will
     * create an animator that is controlled by a timer that updates around
     * 30 Hz.
     *
     * @param controller
     */
    Animator (std::unique_ptr<Controller> controller_ = nullptr);

    ~Animator ();

    /**
     * @brief Set a new controller object, replacing (and destryoying) the
     * current one
     *
     * @param controller
     */
    void setController (std::unique_ptr<Controller> controller);

    /**
     * @brief return a pointer to the active controller object.
     *
     * @return Controller*
     */
    Controller* getController () const;

    /**
     * @brief Attempt to set the controller's frame rate.
     *
     * @param rateInHz
     * @return true if the controller's framerate can be changed, and the requested rate
     * is valid.
     */
    bool setFrameRate (int rateInHz);

    /**
     * @brief get the (reported) frame rate of the controller. This may be
     * the rate that was requested or a rate that's measured; the actual value
     * returned is dependent on the controller object in use
     *
     * @return float, < 0 on error.
     */
    float getFrameRate () const;

    /**
     * @brief Update all active animations with a new time.
     *
     * @param timeInMs
     */
    void gotoTime (juce::int64 timeInMs);

    /**
     * Add a new animation to our list, which will start it going!
     * @param  animation The animation sequence to play.
     * @return           true if added okay.
     */
    bool addAnimation (std::unique_ptr<AnimationType> animation);

    /**
     * Cancel any animations with the specified ID, optionally sending one
     * last update call with all values set to their end positions. ID values
     * aren't required to be unique, so this will check all active animations for
     * a matching value.
     * @param  id                ID of the animation(s) to cancel.
     * @param  moveToEndPosition true to force all values to their end position
     *                           before canceling.
     * @return                   True if at least one animation was canceled.
     */
    bool cancelAnimation (int id, bool moveToEndPosition);

    /**
     * Cancel all active animations.
     * @param  moveToEndPosition True to force all values to their end positions first.
     * @return True if we canceled anything.
     */
    bool cancelAllAnimations (bool moveToEndPosition);

    /**
     * Attempt to get a running animation object by passing in its ID value.
     * @param  id ID of the animation you want. If more than one animation use
     *            the same ID, this will only return the first one found.
     * @return    non-owning pointer (or nullptr if not present). Don't store this
     *            pointer as it may be deleted from beneath you.
     * @sa        getAnimations()
     */
    AnimationType* getAnimation (int id);

    /**
     * Attempt to get all animations that use a specific ID.
     * @param  id         ID to look for.
     * @param  animations Vector to fill with non-owning pointers.
     * @return            number of effects found.
     */
    int getAnimations (int id, std::vector<AnimationType*>& animations);

    /**
     * @brief Pass a new ending value to the animation at `id`, if it is
     *        still in progress. Not all animated value classes support
     *        this operation, so it may silently fail.
     *
     * @param id
     * @param valIndex index of the value within the animation.
     * @param newTarget
     * @return true     If the animation exists; this doesn't indicate that
     *                  we actually succeeded.
     */
    bool updateTarget (int id, int valIndex, float newTarget);

private:
    /**
     * Remove any animations that are complete or canceled from the list.
     * If we end with the list empty, stop the timer
     */
    void cleanup ();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Animator)

    std::unique_ptr<Controller> controller;

    std::vector<std::unique_ptr<AnimationType>> animations;

    /// protect code that might contain data races if updates come
    /// from a different thread.
    juce::CriticalSection mutex;
};

} // namespace friz
