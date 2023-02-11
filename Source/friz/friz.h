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

/*
BEGIN_JUCE_MODULE_DECLARATION

ID:               friz
vendor:           bgporter
version:          2.1.0
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
#include "control/chain.h"
#include "control/controller.h"
#include "control/sequence.h"
#include "curves/animatedValue.h"
#include "curves/constant.h"
#include "curves/easing.h"
#include "curves/linear.h"
#include "curves/parametric.h"
#include "curves/sinusoid.h"
#include "curves/spring.h"
