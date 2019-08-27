/*
 * Copyright (c) 2018 Brett g Porter. 
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


/**
 * @class SubTest
 * @brief An extension to the JUCE UnitTest system. 
 */

class SubTest   : public UnitTest 
{
public:
   SubTest(const String& name, const String& category=String())
   :  UnitTest(name, category)
   ,  fIsSetup(false)
   {
      
   }
   
   /**
    * Perform any common setup actions needed by your sub-tests.
    *
    * Called automatically by the `Test()` method; you shouldn't need to 
    * call this explicitly.  
    *
    * If your test class needs resources that are allocated once for 
    * all of your subtests, you can handle that by overriding the 
    * `UnitTest::initialise()` method.
    *
    * Default does nothing. 
    */
   virtual void Setup()
   {
      
   }
   
   /**
    * Perform any common cleanup needed by your subtests. 
    *
    * If your test class allocated resources in the `initialise()` method that 
    * stayed in scope for all of your subtests, you should handle that cleanup 
    * by overriding the `UnitTest::shutdown()` method. 
    *
    * If the class you're testing has a method that lets you check a class 
    * invariant, adding a call inside the `TearDown()` method like:
    * ```
    *    // call your class invariant checker
    *    expect(this->IsValid());
    * ```
    *
    * Lets you check that each test not only succeeded on its own terms, but 
    * left the object being tested in a valid state. 
    */
   virtual void TearDown()
   {
      
   }
   
   /**
    * The Test method:
    * * performs common setup. 
    * * executes a single subtest 
    * * performs the common cleanup.  
    *
    * Your unit tests will be a sequence of calls to `Test(...)` inside of your 
    * test suite class' `runTest()` method. 
    * 
    * @param testName Name of the sub-test. 
    * @param testFn   Test function to call. 
    * @sa SkipTest
    */
   void Test(StringRef testName, std::function<void(void)> testFn)
   {
      beginTest(testName);
      jassert(! fIsSetup);
      Setup();
      fIsSetup = true;
      
      testFn();
      
      TearDown();
      fIsSetup = false;
   }

   /**
    * An easy way to disable a test that's implemented by calling an std::function 
    * that doesn't just comment (or conditionally compile) out that test. Your 
    * test log will include a line helping you remember that the test wasn't run. 
    *
    * Assumption is that you may encounter working tests that momentarily break 
    * so we want to temporarily disable them. Instead of commenting or `#ifdef`-ing 
    * those tests out, change the function call from `Test(...)` to 
    * `SkipTest(...)`. To re-enable the test, just change that method call back 
    * to `Test(...)`.
    * 
    * @param testName name/description of this test. 
    * @param void     test function to (not) call. 
    *
    * @sa Test
    */
   void SkipTest(StringRef testName, std::function<void(void)> /*testFn*/)
   {
      logMessage ("-----------------------------------------------------------------");
      logMessage(TRANS("WARNING: Skipping ") + this->getName() + " / " + testName);
   }
private:
   bool fIsSetup;
};
