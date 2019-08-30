/*
  ==============================================================================

    demoComponent.h
    Created: 30 Aug 2019 4:22:45pm
    Author:  Brett Porter

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DemoComponent    : public Component
{
public:
    DemoComponent();
    ~DemoComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)
};
