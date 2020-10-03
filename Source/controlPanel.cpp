/*
 * Copyright (c) 2019 Brett g Porter. 
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
   fSlider->setNumDecimalPlacesToDisplay(3);
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

VtComboBox::VtComboBox(ValueTree tree, Identifier param) 
:  fTree(tree)
,  fParam(param)
{
   fCombo = std::make_unique<ComboBox>(param.toString());
   this->addAndMakeVisible(fCombo.get());
   fCombo->addListener(this);
   fCombo->setColour(ComboBox::backgroundColourId, juce::Colour(0x00000000));
   this->setSize(190, 24);
}

void VtComboBox::resized() 
{
   fCombo->setBounds(this->getLocalBounds());
}

void VtComboBox::comboBoxChanged(ComboBox*) 
{
   auto selected = fCombo->getSelectedItemIndex();
   if (selected >= 0)
   {
      fTree.setProperty(fParam, selected, nullptr);
   }   
}

void VtComboBox::AddSelection(int itemId, StringRef label) 
{
   fCombo->addItem(label, itemId+1);
}

void VtComboBox::Update() 
{
   int index = fTree.getProperty(fParam);
   fCombo->setSelectedId(index+1, NotificationType::dontSendNotification);
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
   AddControl(std::make_unique<VtLabel>(true, "Parametric - [click]"));
   AddControl(std::make_unique<VtLabel>(false, "Curve"));

   auto combo{std::make_unique<VtComboBox>(fTree, ID::kCurve)};
   combo->AddSelection(friz::Parametric::kLinear, "Linear");
   combo->AddSelection(friz::Parametric::kEaseInSine, "Sine (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutSine, "Sine (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutSine, "Sine (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInQuad, "Quad (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutQuad, "Quad (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutQuad, "Quad (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInCubic, "Cubic (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutCubic, "Cubic (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutCubic, "Cubic (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInQuartic, "Quartic (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutQuartic, "Quartic (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutQuartic, "Quartic (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInQuintic, "Quintic (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutQuintic, "Quintic (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutQuintic, "Quintic (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInExpo, "Exponential (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutExpo, "Exponential (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutExpo, "Exponential (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInCirc, "Circular (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutCirc, "Circular (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutCirc, "Circular (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInBack, "Back (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutBack, "Back (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutBack, "Back (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInElastic, "Elastic (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutElastic, "Elastic (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutElastic, "Elastic (in/out)");
   combo->AddSelection(friz::Parametric::kEaseInBounce, "Bounce (ease in)");
   combo->AddSelection(friz::Parametric::kEaseOutBounce, "Bounce (ease out)");
   combo->AddSelection(friz::Parametric::kEaseInOutBounce, "Bounce (in/out)");
   combo->Update();  // set the combo box to the current selection.
   AddControl(std::move(combo));

   AddControl(std::make_unique<VtLabel>(false, "Effect Duration"));
   AddControl(std::make_unique<VtSlider>(fTree, 10, 50*5, true, ID::kDuration));
   
   AddControl(std::make_unique<VtLabel>(true, "Ease In - [alt+click]"));
   AddControl(std::make_unique<VtLabel>(false, "X Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseInToleranceX));
   AddControl(std::make_unique<VtLabel>(false, "X Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 0.99f, false, ID::kEaseInSlewX));
   AddControl(std::make_unique<VtLabel>(false, "Y Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseInToleranceY));
   AddControl(std::make_unique<VtLabel>(false, "Y Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 0.99f, false, ID::kEaseInSlewY));
   
   AddControl(std::make_unique<VtLabel>(true, "Ease Out - [shift+click]"));
   AddControl(std::make_unique<VtLabel>(false, "X Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseOutToleranceX));
   AddControl(std::make_unique<VtLabel>(false, "X Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 1.01f, 1.99f, false, ID::kEaseOutSlewX));
   AddControl(std::make_unique<VtLabel>(false, "Y Tolerance"));
   AddControl(std::make_unique<VtSlider>(fTree, 0.01f, 5.f, false, ID::kEaseOutToleranceY));
   AddControl(std::make_unique<VtLabel>(false, "Y Slew"));
   AddControl(std::make_unique<VtSlider>(fTree, 1.01f, 1.99f, false, ID::kEaseOutSlewY));
      
#if JUCE_MAC
   AddControl(std::make_unique<VtLabel>(true, "Spring - [cmd+click]"));
#else
   AddControl(std::make_unique<VtLabel>(true, "Spring - [ctrl+click]"));
#endif   
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
   
