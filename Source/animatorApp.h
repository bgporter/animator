/*
 * Copyright (c) 2019 Brett g Porter. 
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_DEBUG
#define qRunUnitTests
#include "subTest.h"
#endif


namespace ID 
{
   const Identifier kParameters{"params"};
   const Identifier kBreadcrumbs{"breadcrumbs"};
   const Identifier kDuration{"dur"};
   const Identifier kCurve{"curve"};   // int/enum
   
   const Identifier kEaseOutToleranceX{"eotx"};
   const Identifier kEaseOutToleranceY{"eoty"};
   const Identifier kEaseOutSlewX{"eoslewx"};
   const Identifier kEaseOutSlewY{"eoslewy"};
   
   const Identifier kEaseInToleranceX{"eitx"};
   const Identifier kEaseInToleranceY{"eity"};
   const Identifier kEaseInSlewX{"eislewx"};
   const Identifier kEaseInSlewY{"eislewy"};

   const Identifier kSpringToleranceX{"springToleranceX"};
   const Identifier kSpringDampingX{"springDampingX"};
   const Identifier kSpringToleranceY{"springToleranceY"};
   const Identifier kSpringDampingY{"springDampingY"};
   
   
   const Identifier kFadeDelay{"fadeDelay"};    // int 
   const Identifier kFadeDuration{"fadeDur"};   // int 
}