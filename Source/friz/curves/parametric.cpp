//
// Copyright (c) 2020 Brett g Porter. All Rights Reserved.
//

#include "parametric.h"

namespace
{
constexpr float kN1 { 7.5625f };
constexpr float kD1 { 2.75f };

constexpr float kPi { juce::MathConstants<float>::pi };
constexpr float kZeroIsh { 0.001f }; // compare if we're close enough to zero.
constexpr float kOneIsh { 0.999f };  // compare if we're close enough to one.
// from the literature, naming from those sources.
constexpr float kC1 { 1.70158f };
constexpr float kC2 { kC1 * 1.525f };
constexpr float kC3 { kC1 + 1.f };
constexpr float kC4 { 2 * kPi / 3.f };
constexpr float kC5 { 2 * kPi / 4.5f };

float easeOutBounce (float x)
{
    if (x < (1 / kD1))
    {
        return kN1 * x * x;
    }
    else if (x < (2 / kD1))
    {
        x -= (1.5f / kD1);
        return kN1 * x * x + 0.75f;
    }
    else if (x < (2.5 / kD1))
    {
        x -= (2.25f / kD1);
        return kN1 * x * x + 0.9375f;
    }
    else
    {
        x -= (2.65f / kD1);
        return kN1 * x * x + 0.984375f;
    }
}

float easeInBounce (float x)
{
    return 1 - easeOutBounce (1 - x);
}
} // namespace

namespace friz
{

Parametric::Parametric (CurveType type, float startVal, float endVal, int duration)
: TimedValue (startVal, endVal, duration)
{
    switch (type)
    {
        case kEaseInSine:
            curve = [] (float x) { return 1 - cos_f ((x * kPi) / 2); };
            break;

        case kEaseOutSine:
            curve = [] (float x) { return sin_f (x * kPi / 2); };
            break;

        case kEaseInOutSine:
            curve = [] (float x) { return -(cos_f (kPi * x) - 1) / 2; };
            break;

        case kEaseInQuad:
            curve = [] (float x) { return x * x; };
            break;

        case kEaseOutQuad:
            curve = [] (float x) { return 1 - (1 - x) * (1 - x); };
            break;

        case kEaseInOutQuad:
            curve = [] (float x)
            { return (x < 0.5f) ? (2 * x * x) : (1 - pow_f (-2 * x + 2, 2) / 2); };
            break;

        case kEaseInCubic:
            curve = [] (float x) { return x * x * x; };
            break;

        case kEaseOutCubic:
            curve = [] (float x) { return 1 - pow_f (1 - x, 3); };
            break;

        case kEaseInOutCubic:
            curve = [] (float x)
            { return (x < 0.5f) ? 4 * x * x * x : 1 - pow_f (-2 * x + 2, 3) / 2; };
            break;

        case kEaseInQuartic:
            curve = [] (float x) { return x * x * x * x; };
            break;

        case kEaseOutQuartic:
            curve = [] (float x) { return 1 - pow_f (1 - x, 4); };
            break;

        case kEaseInOutQuartic:
            curve = [] (float x)
            { return (x < 0.5f) ? 8 * x * x * x * x : 1 - pow_f (-2 * x + 2, 4) / 2; };
            break;

        case kEaseInQuintic:
            curve = [] (float x) { return x * x * x * x * x; };
            break;

        case kEaseOutQuintic:
            curve = [] (float x) { return 1 - pow_f (1 - x, 5); };
            break;

        case kEaseInOutQuintic:
            curve = [] (float x) {
                return (x < 0.5f) ? 16 * x * x * x * x * x
                                  : 1 - pow_f (-2 * x + 2, 5) / 2;
            };
            break;
        case kEaseInExpo:
            curve = [] (float x)
            { return (x < kZeroIsh) ? 0.f : pow_f (2, 10 * x - 10); };
            break;

        case kEaseOutExpo:
            curve = [] (float x) { return (x > kOneIsh) ? 1.f : 1 - pow_f (2, -10 * x); };
            break;

        case kEaseInOutExpo:
            curve = [] (float x)
            {
                if (x < kZeroIsh)
                    return 0.f;
                else if (x > kOneIsh)
                    return 1.f;
                else if (x < 0.5f)
                    return pow_f (2, 20 * x - 10) / 2;

                return (2 - pow_f (2, -20 * x + 10)) / 2;
            };
            break;

        case kEaseInCirc:
            curve = [] (float x) { return 1 - std::sqrt (1 - pow_f (x, 2)); };
            break;

        case kEaseOutCirc:
            curve = [] (float x) { return std::sqrt (1 - pow_f (x - 1, 2)); };
            break;

        case kEaseInOutCirc:
            curve = [] (float x)
            {
                if (x < 0.5f)
                    return (1 - std::sqrt (1 - pow_f (2 * x, 2))) / 2;

                return 0.5f * std::sqrt (1 - pow_f (-2 * x + 2, 2)) + 1;
            };
            break;

        case kEaseInBack:
            curve = [] (float x) { return (kC3 * x * x * x) - (kC1 * x * x); };
            break;

        case kEaseOutBack:
            curve = [] (float x)
            { return 1 + kC3 * pow_f (x - 1, 3) + kC1 * std::powf (x - 1, 2); };
            break;

        case kEaseInOutBack:
            curve = [] (float x)
            {
                if (x < 0.5f)
                    return 0.5f * (pow_f (2 * x, 2) * ((kC2 + 1) * 2 * x - kC2));

                return 0.5f *
                       (pow_f (2 * x - 2, 2) * ((kC2 + 1) * (x * 2 - 2) + kC2) + 2);
            };
            break;

        case kEaseInElastic:
            curve = [] (float x) -> float
            {
                if (x < kZeroIsh)
                    return 0.f;
                else if (x > kOneIsh)
                    return 1.f;

                return -pow_f (2, 10 * x - 10) * sin_f ((x * 10 - 10.75f) * kC4);
            };
            break;

        case kEaseOutElastic:
            curve = [] (float x) -> float
            {
                if (x < kZeroIsh)
                    return 0.f;
                else if (x > kOneIsh)
                    return 1.f;

                return pow_f (2, -10 * x) * sin_f ((x * 10 - 0.75f) * kC4) + 1;
            };
            break;

        case kEaseInOutElastic:
            curve = [] (float x) -> float
            {
                if (x < kZeroIsh)
                    return 0.f;
                else if (x > kOneIsh)
                    return 1.f;
                else if (x < 0.5f)
                    return 0.5f *
                           -(pow_f (2, 20 * x - 10) * sin_f ((20 * x - 11.125f) * kC5));

                return 0.5f * (pow_f (2, -20 * x + 10) * sin_f (20 * x - 11.125f)) + 1;
            };
            break;

        case kEaseInBounce:
            curve = easeInBounce;
            break;

        case kEaseOutBounce:
            curve = easeOutBounce;
            break;

        case kEaseInOutBounce:
            curve = [] (float x)
            {
                if (x < 0.5f)
                    return 0.5f * (1 - easeOutBounce (1 - 2 * x));

                return 0.5f * (1 + easeOutBounce (2 * x - 1));
            };
            break;

        case kLinear:
        // fall through
        default:
            curve = [] (float x) { return x; };
            break;
    }

}

void Parametric::SetCurve (CurveFn curve_)
{
    curve = curve_;
}

float Parametric::generateNextValue (float progress)
{
    if (progress >= 1.0f)
        return endVal;

    jassert (curve != nullptr);

    // float curvePoint = curve (progress) * distance;
    // return startVal + (curvePoint * (endVal - startVal));
    return scale (curve (progress));
}

} // namespace friz
