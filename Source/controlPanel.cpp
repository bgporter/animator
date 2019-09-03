/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#include "controlPanel.h"

//==============================================================================
ControlPanel::ControlPanel(ValueTree params)

{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("ControlPanel", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void ControlPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}



void ControlPanel::mouseDown(const MouseEvent& e) 
{
   this->sendChangeMessage();
}
