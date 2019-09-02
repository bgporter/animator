/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "animatorApp.h"

class Breadcrumbs : public Component 
{
public:
   Breadcrumbs()
   :  fEnabled(true)
   {
      // ignore all mouse clicks. 
      this->setInterceptsMouseClicks(false, false);
   }
   
   void Enable(bool isEnabled)
   {
      this->Clear();
      fEnabled = isEnabled;
   }
   
   bool IsEnabled() const 
   {
      return fEnabled;
   }
   
   
   void Clear()
   {
      fBreadcrumbs.clear();
   }
   
   
   void AddPoint(float x, float y)
   {
      if (fEnabled)
      {
         fBreadcrumbs.addRectangle(x, y, 2, 2);
      }
   }
   
   void paint(Graphics& g) override
   {
      if (fEnabled)
      {
         g.setColour(Colours::black);
         g.fillPath(fBreadcrumbs);
      }
   }
   
   
   
   
private:
   Path fBreadcrumbs;
   
   bool fEnabled; 
};
