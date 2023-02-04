/*
 * Copyright (c) 2019 Brett g Porter.
 */
#pragma once

#include "animatedValue.h"

namespace friz
{
/**
 * @class Linear
 * @brief Simple linear interpolator.
 */
class Linear : public TimedValue
{
public:
    /**
     * A value that changes linearly.
     * @param startVal  initial value
     * @param endVal    ending value
     * @param tolerance tolerance for completion.
     * @param duration  # of frames the effect should take.
     */
    Linear (float startVal, float endVal, int duration);

private:
    float generateNextValue (float progress) override;
};

} // namespace friz
