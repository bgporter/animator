/*
 * Copyright (c) 2019 Brett g Porter. 
 */

#pragma once

#include "../animatorApp.h"

#include "animation.h"
#include "constant.h"
#include "linear.h"
#include "easing.h"
#include "spring.h"
#include "sequence.h"

namespace friz 
{
/**
 * @class Animator 
 * @brief A class that can own `Animation` objects and execute them at a regular 
 *        interval. 
 *
 * Since the timer that drives the animations only executes if there is currently 
 * an animation happening, there's no overhead when the animator is idle. 
 */


class Animator : public Timer
{
public:
   Animator(int frameRate = 30);
   
   ~Animator();
   
   void timerCallback() override;
   
   /**
    * Convert a time in seconds to the corresponding frame count based on the 
    * current frame rate. 
    * @param  seconds Seconds, must be > 0
    * @return         nearest # of frames, will be >= 1.
    */
   int TimeToFrames(float seconds);
   
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
   
   
   /**
    * Attempt to get a running animation object by passing in its ID value. 
    * @param  id ID of the animation you want. If more than one anumation use 
    *            the same ID, this will only return the first one found. 
    * @return    non-owning pointer (or nullptr if not present). Don't store this 
    *            pointer as it may be deleted from beneath you. 
    */  
   AnimationType* GetAnimation(int id);
   
   /**
    * Attempt to get all animations that use a specific ID. 
    * @param  id         ID to look for. 
    * @param  animations Vector to fill with non-owning pointers. 
    * @return            number of effects found. 
    */
   int GetAnimations(int id, std::vector<AnimationType*>& animations);
   
   
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


} // namespace friz
