/*
 * Copyright (c) 2019 Brett g Porter.
 */

#pragma once

#include "animatedValue.h"

namespace friz
{
/**
 * @class Constant
 * @brief A class that generates a single constant value for its duration.
 *
 * Wait -- why would you need a constant value when you're trying to *animate*?
 *
 * Consider this case: I have an an animation that's cyclic, and I want to be
 * able to set it to run for some number of cycles that I don't know at compile
 * time. An easy way to do this is to create an Animation object that has a
 * `Constant` value in it, and in the `OnCompletion()` callback, check to see if
 * that value is > 0 -- if not, immediately recreate the animation, but decrement
 * the constant loop count value.
 */

class Constant : public TimedValue
{
  public:
    /**
     * A value that doesn't change.
     * @param value      Value to generate.
     * @param duration # of frames the effect should take.
     */
    Constant(int value, int duration);

  private:
    float GenerateNextValue() override;

  private:
};

} // namespace friz
