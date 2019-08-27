#! /usr/bin/env python

'''
   makeTest.py -- tiny utility to create a skeleton JUCE UnitTest class.
   Copyright (c) 2018 Brett g Porter

   Given the command line param 'ClassName', will create a file
   `test_ClassName.cpp` either in :
      - a directory named `test` underneath the current directory.
      - in the current directory, if we're already in a directory named `test`.
   It will also attempt to open a file `className.cpp` and append the necessary
   code to include the code in the newly created test file. 
   
   **NOTE** that the macro `qRunUnitTests` must be defined in the source file
   that wants to be tested.    
   
'''

import os

kTemplate = """
class Test_{0} : public SubTest
{{
public:
   Test_{0}() 
   : SubTest("{0}", "!!! category !!!")
   {{

   }}

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
   {{
    
   }}

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
   {{
    
   }}
   
   void runTest() override
   {{
      // !!! Add tests.
      beginTest("!!! WRITE SOME TESTS FOR THE {0} Class !!!");
   }}

}};

static Test_{0}   test{0};
"""

kCppTemplate = """
#ifdef qRunUnitTests
#include "test/test_{0}.cpp"
#endif
"""


def CreateTestFile(className):
    pth = os.getcwd()
    head, currentDir = os.path.split(pth)
    if currentDir != 'test':
        # We're not already in the test directory.
        try:
            pth = os.path.join(pth, "test")
            print "Creating dir {}".format(pth)
            os.mkdir(pth)
        except OSError:
            # the dir already exists, ignore.
            pass
    fileName = "test_{}.cpp".format(className)
    fullPath = os.path.join(pth, fileName)
    if (not os.path.exists(fullPath)):
       print "Creating file {}".format(fullPath)
       with open(fullPath, "wt") as f:
           f.write(kTemplate.format(className))
       return True
    else:
       print "test file already exists -- not creating it."
       return False

def AddTestInclude(className):
   ''' attempt to open the cpp source file being tested and append the 
      chunk of code that conditionally includes the unit tests.
   '''
   pth = os.getcwd()
   head, currentDir = os.path.split(pth)
   if currentDir == 'test':
      # we are in the test directory already, need to look above where we are.
      pth = head
   
   fileName = className[0].lower() + className[1:] + ".cpp"
   fullPath = os.path.join(pth, fileName);
   
   if os.path.exists(fullPath):
      print "Adding include of the unit tests. "
      with open(fullPath, "at") as f:
         f.write(kCppTemplate.format(className))
      return True
   else:
      print "ERROR: {} not found".format(fullPath)   
      return False
   
   


if __name__ == "__main__":
    import sys
    className = sys.argv[1]
    if CreateTestFile(className):
       AddTestInclude(className)
