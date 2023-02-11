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

#include "animation.h"

namespace friz
{

/**
 * @class Chain
 * @brief A container animation object that holds several animations and
 * executes them in order.
 *
 * This differs from the Sequence class in that the animations in a Chain do not
 * need to use the same number of values.
 *
 * @sa Sequence
 */

class Chain : public AnimationType
{
public:
    Chain (int id = 0)
    : AnimationType (id)
    {
    }

    bool isFinished () override { return (currentEffect >= sequence.size ()); }

    bool isReady () const override
    {
        for (const auto& effect : sequence)
        {
            if ((nullptr == effect) || (!effect->isReady ()))
                return false;
        }
        return true;
    }

    AnimationType::Status gotoTime (juce::int64 timeInMs) override
    {
        auto effect = getEffect (currentEffect);
        if (effect)
        {
            if (AnimationType::Status::finished == effect->gotoTime (timeInMs))
                ++currentEffect;

            return isFinished () ? AnimationType::Status::finished
                                 : AnimationType::Status::processing;
        }
        return AnimationType::Status::finished;
    }

    void cancel (bool moveToEndPosition) override
    {
        currentEffect = static_cast<int> (sequence.size () - 1);
        if (moveToEndPosition)
        {
            auto lastEffectPtr = getEffect (currentEffect);
            if (lastEffectPtr)
                lastEffectPtr->cancel (moveToEndPosition);
        }
    }

    void addAnimation (std::unique_ptr<AnimationType> effect)
    {
        sequence.push_back (std::move (effect));
    }

    AnimatedValue* getValue (size_t index) override
    {
        if (auto effect = getEffect (currentEffect); effect != nullptr)
            return effect->getValue (index);
        return nullptr;
    }

private:
    /**
     * Get a pointer to one of our effects by its index.
     * @param  index 0..size-1
     * @return       nullptr if index is out of range.
     */
    AnimationType* getEffect (int index)
    {
        if (juce::isPositiveAndBelow (index, sequence.size ()))
            return sequence[index].get ();

        jassertfalse;
        return nullptr;
    }

protected:
    /// @brief the vector that owns our chain of effects.
    std::vector<std::unique_ptr<AnimationType>> sequence;

    /// @brief index (into the sequence vector) of the effect that we are currently
    /// processing.
    int currentEffect { 0 };
};

} // namespace friz