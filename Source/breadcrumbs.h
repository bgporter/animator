/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatorApp.h"

class Breadcrumbs : public Component 
{
public:
   Breadcrumbs()
   {
      // ignore all mouse clicks. 
      this->setInterceptsMouseClicks(false, false);
   }
   
   void Clear()
   {
      fBreadcrumbs.clear();
   }
   
   
   void AddPoint(float x, float y)
   {
      fBreadcrumbs.addRectangle(x, y, 2, 2);
   }
   
   void paint(Graphics& g) override
   {
      // PathStrokeType stroke(2);
      g.setColour(Colours::black);
      
      g.fillPath(fBreadcrumbs);
   }
   
   
   
   
private:
   Path fBreadcrumbs;
};