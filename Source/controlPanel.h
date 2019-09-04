/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatorApp.h"


class VtSlider : public Component 
               , public Slider::Listener
{
public:
   VtSlider(ValueTree tree, float min, float max, bool isInt, Identifier param);
   
   void resized() override;
   
   void sliderValueChanged(Slider* s) override;
   
private:
   std::unique_ptr<Slider> fSlider; 
   ValueTree fTree;
   Identifier fParam;
   bool fIsInt;
};


class VtCheck  : public Component 
               , public Button::Listener 
{
public:
   VtCheck(ValueTree tree, Identifier param, StringRef label);
   
   void resized() override;
   
   void buttonClicked(Button* b) override;
   
private:
   std::unique_ptr<ToggleButton> fButton;
   ValueTree fTree;
   Identifier fParam;
   
};


class ControlWell : public Component 
{
public:
   ControlWell(ValueTree params);
   
   void resized() override;
   
   void paint(Graphics& g) override;
   
private:
   
   void AddControl(std::unique_ptr<Component> control);
   
private:
   ValueTree fTree;
   std::vector<std::unique_ptr<Component>> fControls;
   
};

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
   ControlWell fControls;
   

private:
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
   
};
