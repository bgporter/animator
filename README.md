# Friz

A generic UI animation controller for JUCE projects.  

For a fuller discussion of these classes, see my blog post at [https://artandlogic.com/friz-and-the-illusion-of-life/](https://artandlogic.com/friz-and-the-illusion-of-life/). 

## API Docs

API documentation is available [here](https://bgporter.github.io/animator/).

## Overview 

The `friz` project is a set of C++ classes that can be used in projects built with the [JUCE](https://www.juce.com) application framework to add animation effects to user interface elements. Using this library lets you easily specify a stream of timed values that can be used for any purpose in an application, but most frequently for moving or otherwise changing the visual appearance of UI elements in the application. It comes with a rich set of "easing curves" that control how the values are generated over time, and adding your own control curves is simple and straightforward. 

It's named after [Friz Freling](https://en.wikipedia.org/wiki/Friz_Freleng), animator and director of Looney Tunes/Merrie Melodies shorts in the golden age of the Warner Bros. animation studio. 

## Crash Course

Let's imagine a simple use of this: A JUCE applicaton with a main component that has nothing in it. Every time you click inside the window, the background color will smoothly animate to a new, random color. This component needs two member variables, a `juce::Colour` and a `friz::Animator`.

```cpp
class MyContentComponent : public juce::Component 
{
public:
    MyContentComponent (); 

    // called whenever the mouse is clicked in the component
    void mouseDown (const juce::MouseEvent& e) override;

    // flood-fill the component with the active color
    void paint (juce::Graphics& g) override { g.fillAll (bgColor); }
private:
    juce::Colour bgColor { juce::Colours::white };
    friz::Animator animator;   
};
```

We'll hand-wave past all of the "getting a JUCE application that has a main window that can host this as a content component" business -- you should already know how to do that part. 

Without doing anything more, we have a component that is ready to generate and handle animation events. Instantiating a `friz::Animator` with no arguments creates an animation controller that's driven by a `juce::Timer` and will execute at a rate of 30 frames per second when an animation is running. 

To get the color change animation running, we'll implement the `mouseDown` handler that JUCE provides for us: 

```cpp
void MyContentComponent::mouseDown (const juce::MouseEvent& e)
{
    // arbitrary ID value; we use to prevent multiple animations at once. 
    const int animationId { 1 };

    // check to see if our animation is currently running -- if so, just exit:
    if (animator->getAnimation (animationId) != nullptr)
        return;
    // we'll animate the color through HSB color space, so we need some random numbers to go toward: 
    auto& random { Random::getSystemRandom () };
    const auto nextHue { random.nextFloat () };
    const auto nextSat { random.nextFloat () };
    const auto nextBright { random.nextFloat () };
    // a random number of milliseconds for the transition, between 300 and 2000
    const auto duration { random.nextInt { 300, 2000 } };

    // create the animation, using 3 parametric curves with the same settings:
    auto effect { friz::makeAnimation <friz::Parametric, 3> (
        // ID of the animation
        animationId, 
        //  The values to start from, Hue/Saturation/Brightness
        { bgColor.getHue (), bgColor.getSaturation (), bgColor.getBrightness ()}, 
        // The values to animate toward
        { nextHue, nextSat, nextBright}, 
        // # of millseconds for the transition
        duration, 
        // additional args -- here, we specify the shape of the curve. 
        friz::Parametric::kCubic)
        };

    // The animator will execute this callback lambda on every frame:
    effect->updateFn = [this] (int id, const auto& vals)
    {
        // vals is an std::array<float, 3> containing the current in-between state of the 
        // animation. Update the bg color variable and repaint. 
        bgColor = juce::Colour (vals[0], vals[1], vals[2], 1.f);
        repaint ();
    }

    // pass the animation object to the animator, which will start running it immediately. 
    animator.addAnimation (std::move (effect));
}
```

...and that's it. Clicking inside the component will start the animation, run it to completion, and clean up all the objects that it needed when it completes. 

This animation at 30 frames/second is pretty nice, but JUCE timers aren't rock-solid (by design; they really weren't designed for this level of precision). If your app targets versions of JUCE after 7.0.0, you can add a single line of code to make your animations sync exactly to the refresh rate of the monitor, which will make them execute as smoothly as they possibly can:

```cpp
MyContentComponent::MyContentComponent ()
:   animator { std::make_unique<friz::DisplaySyncController> (this) }
{

}
```

This instantiates the animator so that it uses the refresh interval of whatever display the component is being displayed on as its timing source; if you drag your app window from a monitor with a 60Hz refresh rate to a different monitor that updates at 120 Hz, everything adapts automatically for the best performance. 



## Design Goals 

* *Lightweight* If no effects are being run, there's no runtime overhead. 
* *Flexible* Adding new types of animation curves is simple, typically only requiring the creation of a new C++ class derived from an existing effect type and the overriding of a single method. Client code provides a pair of `lambda` objects to receive updates on frame updates and effect completion. Using the `Parametric` class, you can implement an effect by providing a single function that maps a floating point value in the range [0, 1] to a floating point output variable _mostly_ in the range [0, 1] (small excursions outside that range are permitted and can be useful!)
* *Decoupled* Friz doesn't need or want to know anything about your application; it just sends your code back a stream of values at regular intervals.  
* *Modern* Written using current (C++11 and later) capabilities and techniques. 

## Classes 

### `friz::Animator`

[Animator docs](https://bgporter.github.io/animator/classfriz_1_1_animator.html)

Top level animator object, typically owned by a JUCE `Component` object that needs to animate some aspect of one or more child components. 

### `friz::Animation`

[Animation docs](https://bgporter.github.io/animator/classfriz_1_1_animation.html)

An individual instance of a set of animation data. Each animation can provide one or more sets of animation curve data that will be sent back to your code on each frame. A derived class `friz::Sequence` is used to chain multiple animations together as a single logical unit. 

### `friz::AnimatedValue`

[AnimatedValue docs](https://bgporter.github.io/animator/classfriz_1_1_animated_value.html)

Base class for a set of animation curve types that can be instantiated with a start/end value and the definition of when the end value has been reached, either a time duration, or a floating point tolerance to the ending value.

#### Currently Defined Curves

* **Constant**&mdash;emits a stream consisting of the same constant value
* **Linear**&mdash;interpolates linearly between start and end 
* **Parametric**&mdash;provides a set of commonly used easing curves as seen e.g. at https://easings.net
* **Sinusoid**&mdash;generates `sin`/`cos` values between any two phase values
* **EaseIn**&mdash;accelerates quickly away from startVal, decelerates as it approaches endVal
* **EaseOut**&mdash;accelerates slowly away from startVal, accelerates into endVal 
* **Spring**&mdash;accelerates away from startVal. If it overshoots the endVal, will simulate the oscillation of a dampened spring around the endVal until within tolerance. 


## Demo application 

The demonstration application (located in the [`bgporter/frizDemo` repository](https://github.com/bgporter/frizDemo) performs simple animations that 

* apply each of the curve types to animate randomly sized & colored squares away from a mouse click.
* after a delay from the movement animation, fade the square to invisiblity and delete it from the screen. 

The control panel on the right can be hidden and shown by clicking on its gray border (and is itself animated). All of the animation parameters can be tweaked and played with using the sliders in the control panel. 

Additionally, a 'show breadcrumbs' checkbox controls the display of a point on the screen indicating the position of the last square on each frame of the animation&mdash;very helpful for visualizing how each of the curves actually behaves. 

## Other Examples

(Courtesy of [Sudara](https://github.com/sudara)):
This tween animates 2 values at once, the opacity and the position of the component. It uses an AffineTransform for smooth float positioning accuracy. You could also use juce::Component's setBounds methods with integers.

```cpp
auto fadeIn = std::make_unique<friz::Parametric> (friz::Parametric::CurveType::kEaseOutSine, 0.0f, 1.0f, 0.3f * 60);
auto dropIn = std::make_unique<friz::Parametric> (friz::Parametric::CurveType::kEaseOutSine, -10.f, 0.f, 0.5f * 60);
auto animation = std::make_unique<friz::Animation<2>> (friz::Animation<2>::SourceList { std::move (fadeIn), std::move (dropIn) }, 0);
animation->onUpdate ([&] (int id, const auto& val) {
    juce::AffineTransform t;
    this->setTransform (t.translated (0, val[1]));
    this->setAlpha (val[0]);
});
animator.addAnimation (std::move (animation));
```

## Release History

### 2.1.1 Feb 12, 2023

#### Non-breaking Changes

- added new `FRIZ_VBLANK_ENABLED` macro, which will be set to `1` if the version of JUCE being targeted is 7.0.0 or higher. 

### 2.1.0 Feb 11, 2023

#### Non-breaking Changes

##### `Chain` animation type

Add a new `Chain` animation type that lets you bundle together a series of individual animations that will be triggered sequentially. Animations added to a `Chain` each retain their own update and completion callback functions, and do not need to have the same number of Values. 

The existing `Sequence` animation type has been updated so that it's a specialization of the `Chain` type, where all the contained animations must contain the same number of Values, and a single `UpdateFn` callback is used for the entire series of effects. 

Comceptually, a `Chain` lets you say "I want this to happen, and when that's done, I want this other thing to happen." A `Sequence` is used when you want to build a single complex animation out of several simpler effects that happen in order. 

In the demo app, we use a Sequence for the 'pop out' of the sidebar&mdash;we first click the sidebar to the right, then run another animation to pop it out and make it visible. A single callback updates the position of the sidebar component. 

We use a `Chain` when handling the demo boxes; the first animation moves a box from its creation point to some other (x,y) position on screen, and when that's done we start a second animation that performs a linear (1-dimensional) fade of the box's fill color.

##### `makeAnimation` Factory Function

Add a new free function `makeAnimation` to make the creation of many effects much simpler. In practice, most of the effects that I use are either 1-dimensional, or multi-dimensional, but all of the values being generated are using the same curve type and parameters. 

Code that before might have been written like:

```cpp
auto fade = std::make_unique<friz::Animation<1>> (
    friz::Animation<1>::SourceList {
        std::make_unique<friz::Linear> (startValue, endValue, duration) },
                effectId);
```

can be cleaned up into: 

```cpp
auto fade = friz::makeAnimation<friz::Linear>(effectId, startValue, endValue, duration);
```

The benefits become apparent when making multi-dimensional effects, as you can pass all the keyframe values at once: 

```cpp
auto moveEffect { friz::makeAnimation<friz::Parametric, 2> (
    effectId, {0.f, 0.f}, {100.f, 100.f}, 200, friz::Parametric::kCubic)};
```

-- creating an animation object that contains two AnimatedValue obiects, ready to run by passing to an Animator object. 

### 2.0.0 February 5, 2023

#### Breaking Changes

This is a major version bump, and as such, there are breaking changes that will require updates in existing code:

* All functions/method names have been converted from `StudlyCaps` to `mixedCase` to follow JUCE conventions. 
* All durations are now specified in milliseconds instead of frames. It should always have been this way, but without doing this, synching to vertical blanking would have been problematic. 
* new argument added to the completion callback; a boolean `wasCanceled` will be passed to indicate whether the animation is ending normally, or because it was cancelled. 
* New `DisplaySyncController` class uses the `juce::VBlankAttachment` class to synchronize animation updates with the display. 
* The Easing family of `AnimatedValue` (`EaseIn`, `EaseOut`, `Spring`) objects will need much attention with regards to their control values (slew, acceleration, etc.) To support variable frame rates sensibly, all of these curves are now updated _internally_ at a rate of 1000 frames per second, so they should have the same behavior regardless of the actual frame update rate that's in use. 

#### Non-breaking Changes

* Doxygen comments corrected, cleaned up, added as needed
* General cleanup throughout
* MIT license text added at top of all source files. 

### 1.6.0 January 2023
* bug fixes, compiler warnings

### 1.5.0 March 2022

* Corrected bug in the Sinusoid animated value; start value returned when initial phase angle of the object was anything but zero was wrong. 
* Removed demo app code & build files into a separate repo (look for the `bgporter/frizDemo` repo). This repository now contains just the bare JUCE module for inclusion in a JCUE project.

### 1.4.0 November 3, 2021

* Add mutex protection around animator functions that handle the list of running 
  animations. Working on a project where animations are launched, modified from 
  MIDI callback data, and exposed data races. 
* Add `AnimatedValue::UpdateTarget()` virtual method to allow changing 
  an animated value's end state while the animation is in progress. Default
  implementation does nothing. 
* Add `SmoothedValue` class that accepts new end values while the animation is 
  running.  

### 1.3.0 November 6, 2020

Restructured the Animation::Update() logic.

Originally, on each frame, we got the next value from each of the value sources in the animation, and if all of them are in a state of completion, after call the OnUpdate() function, we'd immediately call the OnCompletion() function. 

I've rewritten this so that the OnCompletion function is not called until the **next** frame's Update() call -- I encountered code where calling the completion function too soon after the update function created weird behavior because model code was being updated in the completion callback before the message loop had a chance to process the value set in the preceding Update call. 


### 1.2.0: November 4, 2020

Corrected bug where the EaseIn, EaseOut, and Spring curve classes
all used integer parameters in their constructors for the 
startValue and endValue. 

### 1.1.0: October 2, 2020: Added the new `Parametric` animated value, which supports the following common easing curves:

 -  Linear
 -  Sine (ease in)
 -  Sine (ease out)
 -  Sine (in/out)
 -  Quad (ease in)
 -  Quad (ease out)
 -  Quad (in/out)
 -  Cubic (ease in)
 -  Cubic (ease out)
 -  Cubic (in/out)
 -  Quartic (ease in)
 -  Quartic (ease out)
 -  Quartic (in/out)
 -  Quintic (ease in)
 -  Quintic (ease out)
 -  Quintic (in/out)
 -  Exponential (ease in)
 -  Exponential (ease out)
 -  Exponential (in/out)
 -  Circular (ease in)
 -  Circular (ease out)
 -  Circular (in/out)
 -  Back (ease in)
 -  Back (ease out)
 -  Back (in/out)
 -  Elastic (ease in)
 -  Elastic (ease out)
 -  Elastic (in/out)
 -  Bounce (ease in)
 -  Bounce (ease out)
 -  Bounce (in/out)