/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */


#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
:  fParams(ID::kParameters)
,  fStage(fParams)
{
   fParams.setProperty(ID::kBreadcrumbs, true, nullptr);
   fParams.setProperty(ID::kDuration, 50, nullptr);
   fParams.setProperty(ID::kEaseOutToleranceX, 0.6f, nullptr);
   fParams.setProperty(ID::kEaseOutToleranceY, 0.6f, nullptr);
   fParams.setProperty(ID::kEaseOutSlewX, 1.2f, nullptr);
   fParams.setProperty(ID::kEaseOutSlewY, 1.2f, nullptr);
   fParams.setProperty(ID::kEaseInToleranceX, 0.01f, nullptr);
   fParams.setProperty(ID::kEaseInToleranceY, 0.01f, nullptr);
   fParams.setProperty(ID::kEaseInSlewX, 0.5f, nullptr);
   fParams.setProperty(ID::kEaseInSlewY, 0.5f, nullptr);
   
   
   fParams.setProperty(ID::kSpringDampingX, 0.5f, nullptr);
   fParams.setProperty(ID::kSpringDampingY, 0.5f, nullptr);
   fParams.setProperty(ID::kSpringToleranceX, 0.5f, nullptr);
   fParams.setProperty(ID::kSpringToleranceY, 0.5f, nullptr);
   
   fParams.setProperty(ID::kFadeDelay, 50, nullptr);
   fParams.setProperty(ID::kFadeDuration, 150, nullptr);
   
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
    
    this->addAndMakeVisible(fStage);

}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    fStage.setBounds(this->getLocalBounds());
}
