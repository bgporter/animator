/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"


class Animator : public Timer
{
public:
   Animator();
   
   ~Animator();
   
   /**
    * The Animator should be started once, probably by the 
    * application object immediately after initialization. 
    * @param framesPerSecond Update rate for the animator in Hz.
    */
   void Start(int framesPerSecond=30);

   void timerCallback() override;

   
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Animator)
   
};
