/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */

#pragma once

#include "../animatorApp.h"

#include "animation.h"

class Animator : public Timer
{
public:
   Animator(int frameRate = 30);
   
   ~Animator();
   
   void timerCallback() override;
   
   /**
    * Add a new animation to our list, which will start it going! 
    * @param  animation The animation sequence to play.  
    * @return           true if added okay. 
    */
   bool AddAnimation(std::unique_ptr<AnimationType> animation);
   
   /**
    * Cancel any animations with the specified ID, optionally sending one 
    * last update call with all values set to their end positions. ID values 
    * aren't required to be unique, so this will check all active animations for 
    * a matching value. 
    * @param  id                ID of the animation(s) to cancel. 
    * @param  moveToEndPosition true to force all values to their end position 
    *                           before canceling.  
    * @return                   True if at least one animation was canceled. 
    */
   bool CancelAnimation(int id, bool moveToEndPosition);
   
   /**
    * Cancel all active animations.    
    * @param  moveToEndPosition True to force all values to their end positions first.    
    * @return True if we canceled anything. 
    */
   bool CancelAllAnimations(bool moveToEndPosition); 
   
   
private:
   /**
    * Remove any animations that are complete or canceled from the list. 
    * If we end with the list empty, stop the timer 
    */
   void Cleanup(); 
   

   
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Animator)
    
    std::vector<std::unique_ptr<AnimationType>> fAnimations;
    
    int fFrameRate;
   
};
