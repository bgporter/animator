/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
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
   const Identifier kEaseOutToleranceX{"eotx"};
   const Identifier kEaseOutToleranceY{"eoty"};
   const Identifier kEaseOutSlewX{"eoslewx"};
   const Identifier kEaseOutSlewY{"eoslewy"};
   const Identifier kEaseInToleranceX{"eitx"};
   const Identifier kEaseInToleranceY{"eity"};
   const Identifier kEaseInSlew{"eislew"};
}