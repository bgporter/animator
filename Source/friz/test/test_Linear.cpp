
class Test_Linear : public SubTest
{
public:
   Test_Linear() 
   : SubTest("Linear", "Values")
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
      Test("simple test", [=] {
         auto val = std::make_unique<Linear>(0, 100, 100);
         
         expectWithinAbsoluteError<float>(val->GetNextValue(), 0.f, 0.01f);
         expect(! val->IsFinished());
         
         expectWithinAbsoluteError<float>(val->GetNextValue(), 1.f, 0.01);
         expect(! val->IsFinished());
         
         
         float expected = 1.f;
         int frame = 2;
         
         while (! val->IsFinished())
         {
            expected += 1.f;
            frame++;
            
            expectWithinAbsoluteError<float>(val->GetNextValue(), expected, 0.01);
         }
         expectEquals(frame, 100);
         
      });
      
      Test("Decrease value", [=] {
         auto val = std::make_unique<Linear>(100, 0, 100);
         int frame = 1;
         float expected = 100.f;
         
         while (1)
         {
            expectWithinAbsoluteError<float>(val->GetNextValue(), expected, 0.01);
            if (val->IsFinished())
            {
               break;
            }
            ++frame;
            expected -= 1.f;
         }
         expectEquals(frame, 100);
         
      });
      
      
   }

};

static Test_Linear   testLinear;
