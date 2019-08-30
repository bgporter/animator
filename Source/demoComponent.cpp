/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */



#include "animatorApp.h"
#include "demoComponent.h"



class DemoBox : public Component 
{
public:
   DemoBox()
   {
      Random r;
      fFill = Colour(r.nextFloat(), 0.9f, 0.9f, 1.f);
      fSize = r.nextInt({50, 100});
   }
   
   void paint(Graphics& g) override 
   {
      g.fillAll(fFill);
      auto bounds = this->getLocalBounds();
      g.setColour(Colours::black);
      g.drawRect(bounds, 4);
   }
   
public:
   
   Colour fFill;
   
   int fSize;
   
   
};





//==============================================================================
DemoComponent::DemoComponent()
:  fAnimator(50)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

DemoComponent::~DemoComponent()
{
   this->Clear();
}

void DemoComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::lightgrey);

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void DemoComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void DemoComponent::Clear()
{
   fAnimator.CancelAllAnimations(false);
   this->deleteAllChildren();
}


void DemoComponent::mouseDown(const MouseEvent& e)
{
   if (e.mods.isPopupMenu())
   {
      this->Clear();
   }
   else 
   {
      this->CreateDemo(e.getPosition());
   }
}

void DemoComponent::CreateDemo(Point<int> startPoint)
{
   Random r;
   auto box = new DemoBox(); 
   this->addAndMakeVisible(box);
   box->setBounds(startPoint.x, startPoint.y, box->fSize, box->fSize);
   
   // set the animation parameters. 
/*
   auto yVal = std::make_unique<LinearAnimatedValue>(startPoint.y, 
      (this->getHeight()-box->getHeight()), 0.5, 30);
      
   auto xVal = std::make_unique<LinearAnimatedValue>(startPoint.x, 
      startPoint.x + r.nextInt({-100, 100}), 0.5, 30);
      
   // animation->SetValue(0, xVal);
   animation->SetValue(0, std::make_unique<LinearAnimatedValue>(startPoint.x, 
      startPoint.x + r.nextInt({-100, 100}), 0.5, 30));
   animation->SetValue(1, std::move(yVal));
*/

   int startX = startPoint.x;
   int endX = r.nextInt({0, this->getWidth() - box->getWidth()});
   int startY = startPoint.y;
   int endY = this->getHeight() - box->getHeight();
   
   auto animation = std::make_unique<Animation<2>>(1);
#if 1 
   animation->SetValue(0, std::make_unique<SlewAnimatedValue>(startX, endX, 0.5f, 0.1f));
   animation->SetValue(1, std::make_unique<SlewAnimatedValue>(startY, endY, 0.5f, 0.1f));
#else 
   animation->SetValue(0, std::make_unique<VectorAnimatedValue>(startX, endX, 0.5f, 0.5f, 0.1f));
   animation->SetValue(1, std::make_unique<VectorAnimatedValue>(startY, endY, 0.5f, 2, 0.1f));

#endif
   
   animation->OnUpdate([=] (const Animation<2>::ValueList& val) {
      box->setTopLeftPosition(val[0], val[1]);
   });
   
   fAnimator.AddAnimation(std::move(animation));
   
}
