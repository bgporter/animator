
class Test_EaseIn : public SubTest
{
public:
   Test_EaseIn() 
   : SubTest("EaseIn", "!!! category !!!")
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
   void Setup() override
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
   void TearDown() override
   {
    
   }
   
   void runTest() override
   {
      Test("calculated slew #1", [=] {
         auto val = std::make_unique<EaseIn>(0, 100, 0.5f, 0.5f);
         
         expectWithinAbsoluteError<float>(val->GetNextValue(), 0.f, 0.01f);
         
         // hand-calculated values. 
         std::vector<float> expected{50.f, 75.f, 87.5f, 93.75f, 96.875f, 
            98.4375f, 99.21875f, 99.6094f};
            
         for (auto expVal: expected)
         {
            expect(! val->IsFinished());
            expectWithinAbsoluteError<float>(val->GetNextValue(), expVal, 0.001f);
         }
         expect(val->IsFinished());
         
         
         
         
         
      });
   }

};

static Test_EaseIn   testEaseIn;
