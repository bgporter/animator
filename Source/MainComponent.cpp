/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */


#include "MainComponent.h"

namespace 
{
   const int kOpenPanelWidth{230};
   const int kClosedPanelWidth{30};
}


//==============================================================================
MainComponent::MainComponent()
:  fParams(ID::kParameters)
,  fStage(fParams)
// ,  fControls(fParams)
,  fPanelState(PanelState::kOpen)
{
   fParams.setProperty(ID::kBreadcrumbs, false, nullptr);
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
   
   DBG(fParams.toXmlString());
   
   
   
   this->addAndMakeVisible(fStage);
   
   fControls.reset(new ControlPanel(fParams));
   
   this->addAndMakeVisible(fControls.get());
   fControls->addChangeListener(this);
   setSize(1000, 700);
}

MainComponent::~MainComponent()
{
   fControls->removeChangeListener(this);
   fControls = nullptr;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    

}

void MainComponent::resized()
{
   auto bounds = this->getLocalBounds();
   fStage.setBounds(bounds);
    
   if (fPanelState != PanelState::kClosing || fPanelState != kOpening)
   {
      // handwave past the possibility of resizing while the panel is moving. 
      int panelWidth = kOpenPanelWidth;
      int showing = (PanelState::kClosed == fPanelState) ? kClosedPanelWidth : panelWidth;
      
      int xPos = bounds.getWidth() - showing;
      fControls->setBounds(xPos, 0, kOpenPanelWidth, bounds.getHeight());
   }
    
}




void MainComponent::changeListenerCallback(ChangeBroadcaster* src)
{
   if (src == fControls.get())
   {
      // user clicked on panel -- open or close it. 
      if (PanelState::kOpen == fPanelState)
      {
         this->ClosePanel();
      }
      else if (PanelState::kClosed == fPanelState)
      {
         this->OpenPanel();
      }
      // else, we're already in motion, do nothing. 
      
   }
}



void MainComponent::OpenPanel()
{
   jassert(PanelState::kClosed == fPanelState);
   int width = this->getWidth();
   
   int startX = fControls->getX();
   int endX = width - kOpenPanelWidth;
   
   float slew = 0.4;
   
   auto curve = std::make_unique<EaseIn>(startX, endX, 0.5, slew);
   auto animation = std::make_unique<Animation<1>>();
   animation->SetValue(0, std::move(curve));
   
   animation->OnUpdate([=] (int id, const Animation<1>::ValueList& val){
      fControls->setTopLeftPosition(val[0], 0);
   });
   
   animation->OnCompletion([=] (int id) {
      fPanelState = PanelState::kOpen;
   });
   
   fPanelState = PanelState::kOpening;
   fPanelAnimator.AddAnimation(std::move(animation));
} 

void MainComponent::ClosePanel()
{
   jassert(PanelState::kOpen == fPanelState);
   int width = this->getWidth();
   
   int startX = fControls->getX();
   int endX = width - kClosedPanelWidth;
   
   float accel = 1.4f;
   float dampen = 0.4f;
   
   auto curve = std::make_unique<Spring>(startX, endX, 0.5, accel, dampen);
   auto animation = std::make_unique<Animation<1>>();
   animation->SetValue(0, std::move(curve));
   
   animation->OnUpdate([=] (int id, const Animation<1>::ValueList& val){
      fControls->setTopLeftPosition(val[0], 0);
   });
   
   animation->OnCompletion([=] (int id) {
      fPanelState = PanelState::kClosed;
   });
   
   fPanelState = PanelState::kClosing;
   fPanelAnimator.AddAnimation(std::move(animation));
   
}
