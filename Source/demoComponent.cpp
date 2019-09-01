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
,  fNextEffectId{0}
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
         if (e.mods.isAltDown())
         {
            type = EffectType::kInOut;
         }
         else 
         {
            type = EffectType::kEaseOut;
         }
      }
      else if (e.mods.isAltDown())
      {
         type = EffectType::kEaseIn;
      }
      else if (e.mods.isCommandDown())
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
   
   auto movement = std::make_unique<Animation<2>>(++fNextEffectId);
   
   
   std::unique_ptr<AnimatedValue> xCurve; 
   std::unique_ptr<AnimatedValue> yCurve;
   
   if (EffectType::kLinear == type)
   {
      xCurve = std::make_unique<LinearAnimatedValue>(startX, endX, 0.5f, 50);
      yCurve = std::make_unique<LinearAnimatedValue>(startY, endY, 0.5f, 50);
   }
   else if (EffectType::kEaseOut == type)
   {
      xCurve = std::make_unique<EaseOut>(startX, endX, 0.6f, 0.01f);
      yCurve = std::make_unique<EaseOut>(startY, endY, 0.6f, 0.01f);
   }
   else if (EffectType::kEaseIn == type)
   {
      xCurve = std::make_unique<EaseIn>(startX, endX, 0.6f, 0.1f);
      yCurve = std::make_unique<EaseIn>(startY, endY, 0.6f, 0.1f);
   }
   else if (EffectType::kVector == type)
   {
      auto xAccel = std::abs(endX - startX) / 50.f;
      auto yAccel = std::abs(endY - startY) / 50.f;
      xCurve = std::make_unique<VectorAnimatedValue>(startX, endX, 0.5f, xAccel, 0.3f);
      yCurve = std::make_unique<VectorAnimatedValue>(startY, endY, 0.5f, yAccel, 0.5f);
   }
   else if (EffectType::kInOut == type)
   {
      auto midX = (startX + endX) / 2;
      auto midY = (startY + endY) / 2;
      
      
      auto xCurve1 = std::make_unique<EaseIn>(startX, midX, 2.f, 0.2f);
      auto yCurve1 = std::make_unique<EaseIn>(startY, midY, 2.f, 0.2f);
      auto effect1 = std::make_unique<Animation<2>>();
      effect1->SetValue(0, std::move(xCurve1));
      effect1->SetValue(1, std::move(yCurve1));
      
      auto xCurve2 = std::make_unique<EaseOut>(midX, endX, 0.5f, 0.1f);
      auto yCurve2 = std::make_unique<EaseOut>(midY, endY, 0.5f, 0.1f);
      auto effect2 = std::make_unique<Animation<2>>();
      effect2->SetValue(0, std::move(xCurve2));
      effect2->SetValue(1, std::move(yCurve2));
      
      auto sequence = std::make_unique<Sequence<2>>(++fNextEffectId);
      sequence->AddAnimation(std::move(effect1));
      sequence->AddAnimation(std::move(effect2));
      
      movement = std::move(sequence);
      
   }
   

   if (EffectType::kInOut != type)
   {
      movement->SetValue(0, std::move(xCurve));
      movement->SetValue(1, std::move(yCurve));
   }
   
   // On each update: move this box to the next position on the (x,y) curve.
   movement->OnUpdate([=] (const Animation<2>::ValueList& val) {
      box->setTopLeftPosition(val[0], val[1]);
   });
   
   
   // When the main animation completes: start a second animation that slowly
   // fades the color all the way out. 
   movement->OnCompletion([=] {
      float currentSat = box->GetSaturation();
      
      auto fade = std::make_unique<Animation<1>>(++fNextEffectId); 
      fade->SetValue(0, std::make_unique<LinearAnimatedValue>(currentSat, 0.f, 0.01f, 200));
      // don't start fading until 50 frames have elapsed
      fade->SetDelay(50);
      
      fade->OnUpdate([=] (const Animation<1>::ValueList& val) {
         // every update, change the saturation value of the color. 
         box->SetSaturation(val[0]);
      });
      
      fade->OnCompletion([=] {
         // ...and when the fade animation is complete, delete the box from the 
         //demo component. 
         this->removeChildComponent(box);
         delete box;
         
      });
      
      fAnimator.AddAnimation(std::move(fade));
   });
   
   fAnimator.AddAnimation(std::move(movement));
   
}
