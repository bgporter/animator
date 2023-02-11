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

#include "chain.h"

namespace friz
{
/**
 * @class Sequence
 *
 * @brief An Animation class that can hold multiple Animation objects and
 *        execute them in sequence.
 *
 * An easy way to create complex effects from primitive movements. This differs
 * from the Chain class in that all the animations in a Sequence must have the same
 * ValueCount template parameter, because a single update callback is used for
 * all of the sub-animations' updates.
 *
 * @sa Chain
 */

template <std::size_t ValueCount>
class Sequence : public Chain,
                 public UpdateSource<ValueCount>
{
public:
    Sequence (int id = 0)
    : Chain (id)
    {
    }

    /**
     * @brief Add an animation with the correct number of values to our sequence
     * of effects.
     *
     * @param effect The animation to add.
     */
    void addAnimation (std::unique_ptr<Animation<ValueCount>> effect) 
    {
        // We need to make each of the effects notify us on update or completion,
        // so that we can pass those along to whoever passed in a single
        // lambda to us.
        effect->updateFn =
            [this] (int /*id*/, const typename Animation<ValueCount>::ValueList& val)
        {
            if (this->updateFn != nullptr)
                this->updateFn (this->getId (), val);
        };

        effect->completionFn = [this] (int /*id*/, bool wasCanceled)
        {
            // Each effect in the sequence will notify us, but we only pass
            // along the final one.
            if ((currentEffect == sequence.size () - 1) && this->completionFn != nullptr)
                this->completionFn (this->getId (), wasCanceled);
        };

        Chain::addAnimation (std::move (effect));
    }
};

} // namespace friz
