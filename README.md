# Animator

**Fall 2019 * Bg Porter**

A generic UI animation controller for JUCE projects.  

For a fuller discussion of these classes, see my blog post at [https://artandlogic.com/2019/09/friz-and-the-illusion-of-life/](https://artandlogic.com/2019/09/friz-and-the-illusion-of-life/). 

## Overview 

The `friz` project is a set of C++ classes that can be used in projects built with the [JUCE](https://www.juce.com) application framework to add animation effects to user interface elements. 

It's named after [Friz Freling](https://en.wikipedia.org/wiki/Friz_Freleng), animator and director of Looney Tunes/Merrie Melodies shorts in the golden age of the Warner Bros. animation studio. 

## Design Goals 

* *Lightweight* If no effects are being run, there's no runtime overhead. 
* *Flexible* Adding new types of animation curves is simple, typically only requiring the creation of a new C++ class derived from an existing effect type and the overriding of a single method. Client code provides a pair of `lambda` objects to receive updates on frame updates and effect completion. 
* *Decoupled* Friz doesn't need or want to know anything about your application ; it just sends your code back a stream of values at regular intervals.  
* *Modern* Written using current (C++11 and later) capabilities and techniques. 

## Classes 

### `friz::Animator`

Top level animator object, typically owned by a JUCE `Component` object that needs to animate some aspect of one or more child components. 

### `friz::Animation`

An individual instance of a set of animation data. Each animation can provide one or more sets of animation curve data that will be sent back to your code on each frame. A derived class `friz::Sequence` is used to chain multiple animations together as a single logical unit. 

### `friz::AnimatedValue`

Base class for a set of animation curve types that can be instantiated with a start/end value and the definition of when the end value has been reached, either a set number of frames/time duration, or a floating point tolerance. 

#### Currently Defined Curves

* **Constant**&mdash;emits a stream consisting of the same constant value
* **Linear**&mdash;interpolates linearly between start and end over a set number of frames 
* **Parametric**&mdash;provides a set of commonly used easing curves as seen e.g. at https://easings.net
* **EaseIn**&mdash;accelerates quickly away from startVal, decelerates as it approaches endVal
* **EaseOut**&mdash;accelerates slowly away from startVal, accelerates into endVal 
* **Spring**&mdash;accelerates away from startVal. If it overshoots the endVal, will simulate the oscillation of a dampened spring around the endVal until within tolerance. 
* **Sinusoid**&mdash;generates `sin`/`cos` values between any two phase values


## Demo application 

The included demonstration application performs simple animations that 

* apply each of the curve types to animate randomly sized & colored squares away from a mouse click.
* after a delay from the movement animation, fade the square to invisiblity and delete it from the screen. 

The control panel on the right can be hidden and shown by clicking on its gray border (and is itself animated). All of the animation parameters can be tweaked and played with using the sliders in the control panel. 

Additionally, a 'show breadcrumbs' checkbox controls the display of a point on the screen indicating the position of the last square on each frame of the animation&mdash;very helpful for visualizing how each of the curves actually behaves. 

## Release History

1.2.0: November 4, 2020

Corrected bug where the EaseIn, EaseOut, and Spring curve classes
all used integer parameters in their constructors for the 
startValue and endValue. 

1.1.0: October 2, 2020: Added the new `Parametric` animated value, which supports the following common easing curves:

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