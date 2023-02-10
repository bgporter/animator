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
#include <array>

#include "../curves/animatedValue.h"

namespace friz
{

/**
 * @class AnimationType
 *
 * @brief Abstract base class; all the real action happens in the derived
 *        templated `Animation` class, below.
 */
class AnimationType
{
public:
    enum class Status
    {
        processing, ///< The animation is running right now.
        finished    ///< Finished running, okay to clean up.
    };

    AnimationType (int id)
    : animationId { id }
    , preDelay { 0 }
    {
        // only allow positive animation IDs.
        jassert (id >= 0);
    }

    virtual ~AnimationType () = default;

    /**
     * @return ID value for this Animation.
     */
    int getId () const { return animationId; }

    /**
     * Set a number of frames to delay before starting to execute this animation.
     * @param delay # of delay frames.
     */
    void setDelay (int delay) { preDelay = std::max (0, delay); }

    /**
     * @brief Advance all active animations to this point in time.
     *
     * @param timeInMs Time since some fixed event; only used internally to calculate
     * deltas.
     * @return Status either processing or finished.
     */
    virtual Status gotoTime (juce::int64 timeInMs) = 0;

    /**
     * @brief Cancel an in-progress animation, optionally moving directly to its
     * end value.
     *
     * @param moveToEndPosition if true, go immediately to the end value.
     */
    virtual void cancel (bool moveToEndPosition) = 0;

    /**
     * @return true if the effect has completed.
     */
    virtual bool isFinished () = 0;

    /**
     * @return true if the animation is ready to be executed (e.g. has all its values
     * set to valid AnimatedValue objects.)
     */
    virtual bool isReady () const = 0;

    /**
     * @brief Retrieve a pointer to one of this animation's value objects.
     *
     * @param index
     * @return AnimatedValue*
     */
    virtual AnimatedValue* getValue (size_t index) = 0;

protected:
    /// optional ID value for this animation.
    int animationId { 0 };

    /// an optional pre-delay before beginning to execute the effect.
    int preDelay { 0 };
};

/**
 * @class Animation
 *
 * @brief This class owns a number of `AnimatedValue` objects. On each animation
 * frame it gets the next calculated value from each of the value objects and
 * passes those values to its `OnUpdate` handler. When all of the values in the
 * animation have reached their end states, calls the `OnCompletion` handler.
 *
 * Once this animation is complete, the `Animator` object that owns it will
 * garbage collect it.
 */
template <std::size_t ValueCount> class Animation : public AnimationType
{
public:
    using ValueList  = std::array<float, ValueCount>;
    using SourceList = std::array<std::unique_ptr<AnimatedValue>, ValueCount>;
    using UpdateFn   = std::function<void (int, const ValueList&)>;
    /**
     * @brief callback on completion of this effect
     * @param int id -- ID of this animation.
     * @param bool wasCanceled -- true if the completion is because of cancellation.
     *
     */
    using CompletionFn = std::function<void (int, bool)>;

    /**
     * Create an animation object that can be populated with changing
     * values and functions to call at important points (each frame of animation,
     * sequence completion)
     *
     * @param id Optional identifier, use as you wish. We don't enforce uniqueness,
     *           for example. Must be >= 0.
     */
    Animation (int id = 0)
    : AnimationType { id }
    {
    }

    /**
     * @brief Construct a new Animation object, given a list of value sources.
     *
     * @param sources List of animated value objects.
     * @param id
     */
    Animation (SourceList&& sources, int id = 0)
    : AnimationType { id }
    , sources { std::move (sources) }
    {
    }

    /**
     * Set the AnimatedValue object to use for one of this animation's slots.
     * @param  index Value index, 0..ValueCount-1
     * @param  value AnimatedValue object to generate data.
     * @return       true on success.
     */
    bool setValue (size_t index, std::unique_ptr<AnimatedValue> value)
    {
        if (index >= ValueCount)
        {
            jassertfalse;
            return false;
        }

        sources[index] = std::move (value);
        return true;
    }

    /**
     * @brief Retrieve a pointer to one of this animation's value
     *        sources. This should probably not be used very much, if ever.
     *
     * @param index
     * @return AnimatedValue*
     */
    AnimatedValue* getValue (size_t index) override
    {
        if (index < ValueCount)
            return sources[index].get ();

        jassertfalse;
        return nullptr;
    }

    /**
     * Set the function that will be called with an array of animation values
     * once per frame. `updateFn` is public, so you can also just assign to it directly.
     * @param update UpdateFn function.
     */
    void onUpdate (UpdateFn update) { updateFn = update; }

    /**
     * Set the (optional) function that will be called once when this
     * animation is complete.
     * `completionFn` is public, so you can also just assign to it directly.
     * @param complete CompletionFn function.
     */
    void onCompletion (CompletionFn complete) { completionFn = complete; }

    /**
     * @brief Advance to the specified time, sending value updates to the
     * code that's waiting for them.
     *
     * @param timeInMs number of milliseconds since some fixed event in the past.
     * @return Status, either `processing` or `finished`
     */
    Status gotoTime (juce::int64 timeInMs) override
    {
        if (finished)
        {
            if (completionFn != nullptr)
                completionFn (getId (), false);
            return Status::finished;
        }

        juce::int64 deltaTime;
        // if this is the first time we're being executed, perform some setup:
        if (startTime < 0)
        {
            startTime = lastTime = timeInMs;
            deltaTime            = 0;
        }
        else
        {
            deltaTime = timeInMs - lastTime;
            lastTime  = timeInMs;
        }

        const auto totalElapsed { timeInMs - startTime };

        // if we're still delaying, just return.
        if (totalElapsed < preDelay)
            return Status::processing;

        // recalculate the elapsed and delta times to account for an
        // expired delay
        const auto effectElapsed { totalElapsed - preDelay };
        deltaTime = std::min (deltaTime, totalElapsed);

        // loop through our value generators and update:
        ValueList values;
        int completeCount { 0 };

        for (int i = 0; i < ValueCount; ++i)
        {
            auto& val = sources[i];
            if (val != nullptr)
            {
                // values[i] = val->GetNextValue ();
                values[i] = val->getNextValue (static_cast<int> (effectElapsed),
                                               static_cast<int> (deltaTime));
                completeCount += (val->isFinished ()) ? 1 : 0;
            }
            else
                jassertfalse;
        }

        if (updateFn != nullptr)
            updateFn (getId (), values);

        if (completeCount == ValueCount)
            finished = true;

        return Status::processing;
    }

    void cancel (bool moveToEndPosition) override
    {
        for (auto& val : sources)
        {
            if (val != nullptr)
                val->cancel (moveToEndPosition);
        }

        if (moveToEndPosition && updateFn != nullptr)
        {
            // send one more update where all of the individual values
            // have snapped to their end states.
            ValueList values;
            for (int i = 0; i < ValueCount; ++i)
            {
                auto& val = sources[i];
                jassert (val != nullptr);
                if (val != nullptr)
                    values[i] = val->getEndValue ();
            }
            updateFn (getId (), values);
        }

        // notify that the effect is complete.
        if (completionFn != nullptr)
            completionFn (getId (), true);
        finished = true;
    }

    bool isFinished () override { return finished; }

    bool isReady () const override
    {
        for (auto& src : sources)
        {
            if (nullptr == src.get ())
                return false;
        }
        return true;
    }

public:
    /// function to call on each frame. Pass in std::array of new values,
    /// return true if all is okay, false to cancel this animation.
    UpdateFn updateFn;

    /// function to call when the animation is completed or canceled.
    CompletionFn completionFn;

private:
    /// @brief Timestamp of first update.
    juce::int64 startTime { -1 };
    /// @brief timestamp of most recent update.
    juce::int64 lastTime { -1 };

    /// is this animation complete?
    bool finished { false };

    /// The array of animated value objects.
    SourceList sources;
};

template <class T, int ValueCount, class... Args>
std::unique_ptr<Animation<ValueCount>> makeAnimation (int id,
                                              std::array<float, ValueCount>&& from,
                                              std::array<float, ValueCount>&& to,
                                              Args... args)
{
    // make sure we're trying to create an animation object.
    static_assert (std::is_base_of<AnimatedValue, T>::value);

    auto animation { std::make_unique<Animation<ValueCount>> (id) };

    for (int i { 0 }; i < ValueCount; ++i)
    {
        auto curve { std::make_unique<T> (from[i], to[i], std::forward<Args> (args)...) };
        animation->setValue (i, std::move (curve));
    }

    return animation;
}

// cleaner version to animate a single value:
template <class T, class... Args>
std::unique_ptr<Animation<1>> makeAnimation (int id, float from, float to, Args... args)
{
    return makeAnimation<T, 1> (id, {from}, {to}, std::forward<Args>(args)...);
}

} // namespace friz
