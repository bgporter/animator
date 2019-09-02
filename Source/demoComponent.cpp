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
      fFill = Colour(r.nextFloat(), 0.9f, 0.9f, 0.9f);
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
DemoComponent::DemoComponent(ValueTree params)
:  fParams(params)
,  fAnimator(50)
,  fNextEffectId{0}
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    this->addAndMakeVisible(fBreadcrumbs);
    fBreadcrumbs.toBack();

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
   fBreadcrumbs.setBounds(this->getLocalBounds());
}

void DemoComponent::Clear()
{
   fAnimator.CancelAllAnimations(false);
#if 1
   fBoxList.clear();
   fBreadcrumbs.Clear();
   this->repaint();
#else 
   this->deleteAllChildren();
#endif
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
   // auto box = std::make_unique<DemoBox>(); 
   auto box = new DemoBox();
   fBreadcrumbs.Clear();
   this->repaint();
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
   
   int duration = fParams.getProperty(ID::kDuration, 50);
   
   if (EffectType::kLinear == type)
   {
      xCurve = std::make_unique<Linear>(startX, endX, 0.5f, duration);
      yCurve = std::make_unique<Linear>(startY, endY, 0.5f, duration);
   }
   else if (EffectType::kEaseOut == type)
   {
      float xTolerance = fParams.getProperty(ID::kEaseOutToleranceX, 0.1f);
      float xSlew = fParams.getProperty(ID::kEaseOutSlewX, 1.1f);
      xCurve = std::make_unique<EaseOut>(startX, endX, xTolerance, xSlew);
      
      float yTolerance = fParams.getProperty(ID::kEaseOutToleranceY, 0.1f);
      float ySlew = fParams.getProperty(ID::kEaseOutSlewY, 1.1f);
      yCurve = std::make_unique<EaseOut>(startY, endY, yTolerance, ySlew);
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
      xCurve = std::make_unique<Spring>(startX, endX, 0.5f, xAccel, 0.3f);
      yCurve = std::make_unique<Spring>(startY, endY, 0.5f, yAccel, 0.5f);
   }
   else if (EffectType::kInOut == type)
   {
      auto midX = (startX + endX) / 2;
      auto midY = (startY + endY) / 2;
      
      
      auto xCurve1 = std::make_unique<EaseIn>(startX, midX, 5.f, 0.1f);
      auto yCurve1 = std::make_unique<EaseIn>(startY, midY, 5.f, 0.1f);
      auto effect1 = std::make_unique<Animation<2>>();
      effect1->SetValue(0, std::move(xCurve1));
      effect1->SetValue(1, std::move(yCurve1));
      
      auto xCurve2 = std::make_unique<EaseOut>(midX, endX, 0.5f, 1.3f);
      auto yCurve2 = std::make_unique<EaseOut>(midY, endY, 0.5f, 1.3f);
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
   movement->OnUpdate([=] (int id, const Animation<2>::ValueList& val) {
      box->setTopLeftPosition(val[0], val[1]);
      fBreadcrumbs.AddPoint(val[0], val[1]);
   });
   
   
   // When the main animation completes: start a second animation that slowly
   // fades the color all the way out. 
   movement->OnCompletion([=] (int id){
      float currentSat = box->GetSaturation();
      
      auto fade = std::make_unique<Animation<1>>(++fNextEffectId); 
      fade->SetValue(0, std::make_unique<Linear>(currentSat, 0.f, 0.01f, 200));
      // don't start fading until 50 frames have elapsed
      fade->SetDelay(50);
      
      fade->OnUpdate([=] (int id, const Animation<1>::ValueList& val) {
         // every update, change the saturation value of the color. 
         box->SetSaturation(val[0]);
      });
      
      fade->OnCompletion([=] (int id){
         // ...and when the fade animation is complete, delete the box from the 
         //demo component. 
         DBG("Completing # " << id);
#if 1
         fBoxList.erase(std::remove_if(fBoxList.begin(), fBoxList.end(),
          [&] (const std::unique_ptr<DemoBox>& b){
             return (b.get() == box);
         }));
#else 
         this->removeChildComponent(box);
         delete box;
#endif
         
      });
      
      fAnimator.AddAnimation(std::move(fade));
   });
   
   fAnimator.AddAnimation(std::move(movement));


   fBoxList.emplace_back(box);
   
}
