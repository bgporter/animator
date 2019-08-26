/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"


class AnimatedValue;

template <std::size_t valueCount>
class AnimationController 
{
public:
   AnimationController()
   {
      
   }
   
   /**
    * [Update description]
    * @param  msTime [description]
    * @return        [description]
    */
   int Update(int msTime)
   {
      return 0;
   }
   
   
   
private:
   
   std::array<std::unique_ptr<AnimatedValue>, valueCount> fValues;
   
   /// function to call on each frame. Pass in std::array of new values, 
   /// return true if all is okay, false to cancel this animation. 
   std::function<bool(std::array<float, valueCount>)> fUpdateFn;
   
   /// function to call when the animation is completed or canceled. 
   std::function<void(void)> fCompleteFn;
   
   
   
};