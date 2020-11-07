/*
 * Copyright (c) 2019 Brett g Porter. 
 */
#pragma once

/*
BEGIN_JUCE_MODULE_DECLARATION

ID:               friz
vendor:           bgporter 
version:          1.3.0
name:             'friz' animator
description:      Animation control classes for JUCE.    
website:          https://github.com/bgporter/animator
license:          MIT

dependencies:     juce_gui_basics, juce_core, juce_events

END_JUCE_MODULE_DECLARATION

 */

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include "control/animation.h"
#include "control/animator.h"
#include "curves/animatedValue.h"
#include "curves/constant.h"
#include "curves/easing.h"
#include "curves/linear.h"
#include "curves/parametric.h"
#include "curves/sequence.h"
#include "curves/sinusoid.h"
#include "curves/spring.h"

