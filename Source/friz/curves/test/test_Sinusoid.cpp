
class Test_Sinusoid : public SubTest
{
public:
   Test_Sinusoid() 
   : SubTest("Sinusoid", "!!! category !!!")
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
      Test("minimal sinusoid test", [=] {
         // simple object -- should generate 1 value at each of 
         // 0, pi/2, pi, 3pi/2, 2pi
         auto sin = std::make_unique<Sinusoid>(0, 4, 5);
         
         expectWithinAbsoluteError<float>(sin->GetNextValue(), 0.f, 0.001);
         expect(! sin->IsFinished());
         
         expectWithinAbsoluteError<float>(sin->GetNextValue(), 1.f, 0.001);
         expect(! sin->IsFinished());
         
         expectWithinAbsoluteError<float>(sin->GetNextValue(), 0.f, 0.001);
         expect(! sin->IsFinished());
         
         expectWithinAbsoluteError<float>(sin->GetNextValue(), -1.f, 0.001);
         expect(! sin->IsFinished());
         
         expectWithinAbsoluteError<float>(sin->GetNextValue(), 0.f, 0.001);
         expect(sin->IsFinished());
      });
   }

};

static Test_Sinusoid   testSinusoid;
