/*
 * Copyright (c) 2019 Brett g Porter. All Rights Reserved.
 */
#include "linear.h"



Linear::Linear(float startVal, float endVal, int duration)
:  TimedValue(startVal, endVal, duration) 
{
   jassert(duration > 0);
   fDelta = (fEndVal - fStartVal) / fDuration;
}



float Linear::GenerateNextValue()
{
   auto calculated = fCurrentVal + fDelta;
   if (fStartVal < fEndVal)
   {
      return jmin(calculated, fEndVal);
   }
   return jmax(calculated, fEndVal);
}

#ifdef qRunUnitTests
#include "test/test_Linear.cpp"
#endif
