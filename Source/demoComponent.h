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

#include "breadcrumbs.h"


class DemoBox;

class DemoComponent    : public Component
{
public:
   enum class EffectType 
   {
      kLinear = 0,
      kEaseIn,
      kEaseOut,
      kSpring, 
      kInOut
   };
    DemoComponent(ValueTree params);
    ~DemoComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDown(const MouseEvent& e) override;
    
    void CreateDemo(Point<int> startPoint, EffectType type);
    
    void Clear(); 

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)
    
    ValueTree fParams;
    
    friz::Animator fAnimator;
    Breadcrumbs fBreadcrumbs;
    
    std::vector<std::unique_ptr<DemoBox>> fBoxList;
    
    int fNextEffectId;
};
