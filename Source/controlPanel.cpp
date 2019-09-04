/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#include "controlPanel.h"

//==============================================================================
ControlPanel::ControlPanel(ValueTree params)
:  fControls(params)
{
   this->addAndMakeVisible(fControls);
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

    g.fillAll (Colours::grey);   // clear the background

    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void ControlPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    auto bounds = this->getLocalBounds();
    bounds.removeFromLeft(30);
    fControls.setBounds(bounds);
    
    
}



void ControlPanel::mouseDown(const MouseEvent& e) 
{
   this->sendChangeMessage();
}



VtSlider::VtSlider(ValueTree tree, float min, float max, bool isInt, Identifier param)
:  fTree(tree)
,  fParam(param)
,  fIsInt(isInt)
{
   fSlider.reset(new Slider(param.toString()));
   this->addAndMakeVisible(fSlider.get());
   fSlider->setRange(min, max, isInt ? 1 : 0);
   fSlider->setSliderStyle(Slider::LinearHorizontal);
   fSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);
   fSlider->addListener(this);

   float val = fTree.getProperty(param);

   fSlider->setValue(val, NotificationType::dontSendNotification);
   this->setSize(190, 24);
}

void VtSlider::resized()
{
   fSlider->setBounds(this->getLocalBounds());
}

void VtSlider::sliderValueChanged(Slider* s)
{
   if (fIsInt)
   {
      fTree.setProperty(fParam, static_cast<int>(fSlider->getValue() + 0.5f), nullptr);
   }
   else 
   {
      fTree.setProperty(fParam, fSlider->getValue(), nullptr);
   }
}


VtCheck::VtCheck(ValueTree tree, Identifier param, StringRef label)
:  fTree(tree)
,  fParam(param) 
{
   fButton.reset(new ToggleButton(param.toString()));
   this->addAndMakeVisible(fButton.get());
   fButton->setButtonText(label);
   fButton->addListener(this);
   
   bool isSet = fTree.getProperty(param);
   
   fButton->setToggleState(isSet, NotificationType::dontSendNotification);
   
   this->setSize(190, 24);
}

void VtCheck::resized() 
{
   fButton->setBounds(this->getLocalBounds());
}

void VtCheck::buttonClicked(Button* b)
{
   fTree.setProperty(fParam, fButton->getToggleState(), nullptr);
}

class VtLabel : public Component 
{
public:
   VtLabel(bool large, StringRef text)
   {
      fLabel.reset(new Label(text, text));
      this->addAndMakeVisible(fLabel.get());
      float fontSize = large ? 15.f : 10.f;
      fLabel->setFont(Font(fontSize, Font::plain).withTypefaceStyle("Regular"));
      fLabel->setJustificationType(Justification::centredLeft);
      fLabel->setEditable(false, false, false);
      fLabel->setColour(TextEditor::textColourId, Colours::black);
      fLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
      
      this->setSize(190, large ? 24 : 12);
      
   }
   
   void resized() override 
   {
      fLabel->setBounds(this->getLocalBounds());
   }
   
private:
   std::unique_ptr<Label> fLabel;
   
};
   



ControlWell::ControlWell(ValueTree params)
:  fTree(params)
{
   AddControl(std::make_unique<VtCheck>(fTree,ID::kBreadcrumbs, "Show Breadcrumbs"));
   AddControl(std::make_unique<VtLabel>(true, "Linear"));
   AddControl(std::make_unique<VtLabel>(false, "Effect Duration"));
   AddControl(std::make_unique<VtSlider>(fTree, 10, 50*5, true, ID::kDuration));
   
   AddControl(std::make_unique<VtLabel>(true, "Ease In"));
   AddControl(std::make_unique<VtLabel>(false, "X Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseInToleranceX));
   AddControl(std::make_unique<VtLabel>(false, "X Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 0.99f, false, ID::kEaseInSlewX));
   AddControl(std::make_unique<VtLabel>(false, "Y Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseInToleranceY));
   AddControl(std::make_unique<VtLabel>(false, "Y Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 0.99f, false, ID::kEaseInSlewY));
   
   AddControl(std::make_unique<VtLabel>(true, "Ease Out"));
   AddControl(std::make_unique<VtLabel>(false, "X Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseOutToleranceX));
   AddControl(std::make_unique<VtLabel>(false, "X Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 1.01f, 1.99f, false, ID::kEaseOutSlewX));
   AddControl(std::make_unique<VtLabel>(false, "Y Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseOutToleranceY));
   AddControl(std::make_unique<VtLabel>(false, "Y Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 1.01f, 1.99f, false, ID::kEaseOutSlewY));
      
   AddControl(std::make_unique<VtLabel>(true, "Spring"));
   AddControl(std::make_unique<VtLabel>(false, "X Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kSpringToleranceX));
   AddControl(std::make_unique<VtLabel>(false, "X Damping"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 0.99f, false, ID::kSpringDampingX));
   AddControl(std::make_unique<VtLabel>(false, "Y Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kSpringToleranceY));
   AddControl(std::make_unique<VtLabel>(false, "Y Damping"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 0.99f, false, ID::kSpringDampingY));
   
   AddControl(std::make_unique<VtLabel>(true, "Fade"));
   AddControl(std::make_unique<VtLabel>(false, "Fade Delay"));
   AddControl(std::make_unique<VtSlider>(fTree, 0, 50*5, true, ID::kFadeDelay));
   AddControl(std::make_unique<VtLabel>(false, "Fade Duration"));
   AddControl(std::make_unique<VtSlider>(fTree, 10, 50*5, true, ID::kFadeDuration));
   
}



   
void ControlWell::AddControl(std::unique_ptr<Component> control)
{
   this->addAndMakeVisible(control.get());
   fControls.push_back(std::move(control));
}

void ControlWell::resized() 
{
   int yPos{0};
   int w = this->getWidth();
   // int h = this->getHeight();
   
   for (auto& c: fControls)
   {
      auto h = c->getHeight();
      c->setBounds(2, yPos, w, h);
      yPos += h;
   }
   
}

void ControlWell::paint(Graphics& g) 
{
   g.fillAll(Colour(0xFF505050));
}
   
