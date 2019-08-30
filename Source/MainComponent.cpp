/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */


#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(1000, 700);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
