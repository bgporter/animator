/*
 * Copyright (c) 2019 Brett g Porter. 
 */

#include "animatorApp.h"

#include "demoComponent.h"
#include "controlPanel.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public Component
                     , public ChangeListener
{
public:
   MainComponent();
   ~MainComponent();

   void paint (Graphics&) override;
   void resized() override;
   
   void changeListenerCallback(ChangeBroadcaster* src) override;

private:
   void OpenPanel(); 
   
   void ClosePanel();
private:
   ValueTree fParams;

   DemoComponent fStage;
   std::unique_ptr<ControlPanel> fControls;
   
   friz::Animator fPanelAnimator;
   

   enum PanelState 
   {
      kClosed = 0, 
      kOpen, 
      kOpening, 
      kClosing
   };
   
   PanelState fPanelState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
