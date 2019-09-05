

// include some animated value classes for testing...

} // close the friz namespace for a second. 
#include "../constant.h"


// and re-open it. 
namespace friz 
{
   
class Test_Animation : public SubTest
{
public:
   Test_Animation() 
   : SubTest("Animation", "!!! category !!!")
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
      
      Test("Create animation", [=] {
         float val0{0};
         float val1{0};
         bool isComplete{false};
         
         
         auto control = std::make_unique<Animation<2>>();
         
         expect(! control->IsReady());
         control->SetValue(0, std::make_unique<Constant>(100, 2));
         expect(! control->IsReady());
         control->SetValue(1, std::make_unique<Constant>(200, 3));
         expect(control->IsReady());
         
         control->OnUpdate([&] (int id, const Animation<2>::ValueList& val) {
            val0 = val[0];
            val1 = val[1];
         });
         
         control->OnCompletion([&] (int id){
            isComplete = true;
         });
         
         control->Update();
         expectWithinAbsoluteError<float>(val0, 100.f, 0.01f);
         expectWithinAbsoluteError<float>(val1, 200.f, 0.01f);
         expect(! isComplete);
         
         control->Update();
         expect(! isComplete);
         
         control->Update();
         expect(isComplete);
         
      });


   }

};

static Test_Animation   testAnimation;
