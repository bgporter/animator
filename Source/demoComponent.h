/*
  ==============================================================================

    demoComponent.h
    Created: 30 Aug 2019 4:22:45pm
    Author:  Brett Porter

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "animator/animator.h"

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
    
    void mouseDown(const MouseEvent& e) override;
    
    void CreateDemo(Point<int> startPoint);
    
    void Clear(); 

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)
    
    Animator fAnimator;
};
