
class Test_Animator : public SubTest
{
public:
   Test_Animator() 
   : SubTest("Animator", "Animator")
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
      fAnimator = std::make_unique<Animator>();
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
      fAnimator.reset(nullptr);
   }
   
   void runTest() override
   {
      Test("GetOne", [=] {
         
      fAnimator->AddAnimation(this->MakeNullAnimation(5));
      expect(nullptr == fAnimator->GetAnimation(1));
      expect(nullptr != fAnimator->GetAnimation(5));
         
      });
      
      
      Test("Get multiple", [=] {
         fAnimator->AddAnimation(this->MakeNullAnimation(2));
         fAnimator->AddAnimation(this->MakeNullAnimation(2));
         fAnimator->AddAnimation(this->MakeNullAnimation(2));
         
         
         fAnimator->AddAnimation(this->MakeNullAnimation(100));
         fAnimator->AddAnimation(this->MakeNullAnimation(100));
         
         
         fAnimator->AddAnimation(this->MakeNullAnimation(315));
         
         std::vector<AnimationType*> found;
         
         expectEquals(fAnimator->GetAnimations(5, found), 0);
         expect(0 == found.size());
         
         found.clear();
         expectEquals(fAnimator->GetAnimations(2, found), 3);
         expect(3 == found.size());
         
         // don't clear! 
         expectEquals(fAnimator->GetAnimations(100, found), 2);
         expect(5 == found.size());
         
         found.clear();
         expectEquals(fAnimator->GetAnimations(315, found), 1);
         expect(1 == found.size());
         
         
         
         
      });
   }


   std::unique_ptr<AnimationType> MakeNullAnimation(int id)
   {
      auto val = std::make_unique<Constant>(0, 1);
      auto animation = std::make_unique<Animation<1>>(id); 
      animation->SetValue(0, std::move(val));
      
      return animation;
      
      
   }

private:
   
   std::unique_ptr<Animator> fAnimator;

};

static Test_Animator   testAnimator;
