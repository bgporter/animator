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
      int size = r.nextInt({50, 100});
      this->setSize(size, size);
   }
   
   void paint(Graphics& g) override 
   {
      g.fillAll(fFill);
      auto bounds = this->getLocalBounds();
      g.setColour(Colours::black);
      g.drawRect(bounds, 4);
   }
   
   float GetSaturation() const 
   {
      return fFill.getSaturation();
   }
   
   void SetSaturation(float newSaturation)
   {
      fFill = fFill.withSaturation(newSaturation);
      this->repaint();
   }
   
public:
   
   Colour fFill;
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
      EffectType type = EffectType::kLinear;
      
      if (e.mods.isShiftDown())
      {
         type = EffectType::kSlew;
      }
      else if (e.mods.isAltDown())
      {
         type = EffectType::kVector;
      }
      
      this->CreateDemo(e.getPosition(), type);
   }
}

void DemoComponent::CreateDemo(Point<int> startPoint, EffectType type)
{
   Random r;
   auto box = new DemoBox(); 
   this->addAndMakeVisible(box);
   box->setBounds(startPoint.x, startPoint.y, box->getWidth(), box->getHeight());
   
   // set the animation parameters. 
   int startX = startPoint.x;
   int endX = r.nextInt({0, this->getWidth() - box->getWidth()});
   int startY = startPoint.y;
   int endY = r.nextInt({0, this->getHeight() - box->getHeight()});
   
   auto animation = std::make_unique<Animation<2>>(1);
   
   
   std::unique_ptr<AnimatedValue> xCurve; 
   std::unique_ptr<AnimatedValue> yCurve;
   
   if (EffectType::kLinear == type)
   {
      xCurve = std::make_unique<LinearAnimatedValue>(startX, endX, 0.5f, 50);
      yCurve = std::make_unique<LinearAnimatedValue>(startY, endY, 0.5f, 50);
   }
   else if (EffectType::kSlew == type)
   {
      xCurve = std::make_unique<SlewAnimatedValue>(startX, endX, 0.6f, 0.1f);
      yCurve = std::make_unique<SlewAnimatedValue>(startY, endY, 0.6f, 0.1f);
   }
   else if (EffectType::kVector == type)
   {
      xCurve = std::make_unique<VectorAnimatedValue>(startX, endX, 0.5f, 0.5f, 0.3f);
      yCurve = std::make_unique<VectorAnimatedValue>(startY, endY, 0.5f, 2, 0.3f);
   }
   
   animation->SetValue(0, std::move(xCurve));
   animation->SetValue(1, std::move(yCurve));
   
   animation->OnUpdate([=] (const Animation<2>::ValueList& val) {
      box->setTopLeftPosition(val[0], val[1]);
   });
   
   
   animation->OnCompletion([=] {
      float currentSat = box->GetSaturation();
      
      auto fade = std::make_unique<Animation<1>>(); 
      fade->SetValue(0, std::make_unique<LinearAnimatedValue>(currentSat, 0.f, 0.01f, 200));
      fade->OnUpdate([=] (const Animation<1>::ValueList& val) {
         box->SetSaturation(val[0]);
      });
      
      fade->OnCompletion([=] {
         this->removeChildComponent(box);
         delete box;
      });
      
      fAnimator.AddAnimation(std::move(fade));
   });
   
   fAnimator.AddAnimation(std::move(animation));
   
}
