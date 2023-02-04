// Copyright (c) 2023 Brett g Porter

#include "controller.h"

namespace friz
{
juce::int64 Controller::getCurrentTime ()
{
    auto nowInTix { juce::Time::getHighResolutionTicks () };
    auto nowInMs { 1000 * juce::Time::highResolutionTicksToSeconds (nowInTix) };
    return static_cast<juce::int64> (nowInMs + 0.5);
}
} // namespace friz