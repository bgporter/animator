/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatorApp.h"

class ControlPanel   : public Component
                     , public ChangeBroadcaster
{
public:
   
   ControlPanel(ValueTree params);
   ~ControlPanel();

   void paint (Graphics&) override;
   void resized() override;
   
   void mouseDown(const MouseEvent& e) override;

private:
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
   
   
   
   
};
