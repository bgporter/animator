//
// Copyright (c) 2020 Brett g Porter. All Rights Reserved.
//

#include "parametric.h"
namespace friz
{


Parametric::Parametric(CurveType type, float startVal, float endVal, int duration) 
:  TimedValue(startVal, endVal, duration) 
{
   CurveFn curve; 

   const float kPi = juce::MathConstants<float>::pi;

   switch (type) 
   {
      case kEaseInSine: 
      {
         curve = [=](float x){ return 1 - std::cos((x * kPi) / 2);};
      }
      break;
      case kEaseOutSine: 
      {
         curve = [=](float x){ return std::sin(x * kPi / 2);};
      }
      break;
      case kEaseInOutSine: 
      {
         curve = [=](float x){ return -(std::cos(kPi* x) - 1) / 2;};
      }
      break;
      case kEaseInQuad:
      {
         curve = [](float x){ return x * x;};
      }
      break;
      case kEaseOutQuad: 
      {
         curve = [](float x) { return 1 - (1-x) * (1-x); };
      }
      break;
      case kEaseInOutQuad:
      {
         curve = [](float x) { 
            return   (x < 0.5f) ? 
                     2 * x * x  :
                     1 - std::pow(-2 * x + 2, 2) / 2;  };
      }
      break;

      case kEaseInCubic: 
      {
         curve = [](float x){ return x * x * x;};
      }
      break;

      case kEaseOutCubic:
      {
         curve = [](float x) { return 1 - std::pow(1-x, 3);};
      }
      break;
      
      case kEaseInOutCubic: 
      {
         curve = [](float x) { 
            return   (x < 0.5f) ? 
                     4 * x * x * x :
                     1 - std::pow(-2 * x + 2, 3) / 2;  };
      }
      break;

      case kEaseInQuartic: 
      {
         curve = [](float x){ return x * x * x * x;};
      }
      break;

      case kEaseOutQuartic:
      {
         curve = [](float x) { return 1 - std::pow(1-x, 4);};
      }
      break;
      case kEaseInOutQuartic:
      {
         curve = [](float x) { 
            return   (x < 0.5f) ? 
                     8 * x * x * x * x :
                     1 - std::pow(-2 * x + 2, 4) / 2;  };
      }
      break;
      case kEaseInQuintic:
      case kEaseOutQuintic: 
      case kEaseInOutQuintic:
      case kEaseInExpo:
      case kEaseOutExpo:
      case kEaseInOutExpo:
      case kEaseInCirc:
      case kEaseOutCirc:
      case kEaseInOutCirc:
      case kEaseInBack:
      case kEaseOutBack:
      case kEaseInOutBack:
      case kEaseInElastic:
      case kEaseOutElastic:
      case kEaseInOutElastic:
      case kEaseInBounce: 
      case kEaseOutBounce: 
      case kEaseInOutBounce:

      case kLinear:
      // fall through
      default:
      {
         curve = [](float x) { return x; };
      }
      break;



   }


   this->SetCurve(curve);

   fDistance = std::max(startVal, endVal) - std::min(startVal, endVal);

   
}

void Parametric::SetCurve(CurveFn curve) 
{
   fCurve = curve;
}

float Parametric::GenerateNextValue() 
{
   float progress = (1.f * fFrameCount) / (fDuration - 1);
   float curvePoint = fCurve(progress) * fDistance;

   if (fEndVal > fStartVal)
   {
      return fStartVal + curvePoint;
   }
   return fStartVal - curvePoint;
}





}