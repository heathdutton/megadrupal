SN SimpleTest Plus Description
====================================
SN SimpleTest Plus Drupal module.  Please see
the following page for more current information:

https://drupal.org/sandbox/pgautam/2080875

As this module is extension for SimpleTest module.
Please see the following page for more information on SimpleTest module:

https://drupal.org/simpletest-tutorial-drupal7 

 Installation
====================================
Regular Drupal module installation.  You
can create SimpleTest skeleton for chosen module:

admin/config/system/add-simple-test/modules-list

Usage
====================================
This module is written for developers to generate test skeleton for different 
modules they are using in their application.

Steps to generate and run simpletest:
1) Download and Install "SN SimpleTest Plus" Module.

2) Generate test for any module using link:
   admin/config/system/add-simple-test/modules-list

3) Check your module directory for "YOUR_MODULE_NAME.test" file which should
   contain multiple functions based on cylomatic complexity (IF/ELSE, SWITCH) 
   in your code.

   Eg:
   // Sample module for generating test
   File: YOUR_MODULE.module
   
   /**
    * Function to generate hello world
    */
    function YOUR_MODULE_say_hello($msg = "Hello World") {
      if($msg == "Hello World") {
        drupal_set_message($msg);
      }
    }
   
    // After module completion generate test skeleton
    File: YOUR_MODULE.test
    
    // Code for YOUR_MODULE test will look like
     /**
      * Tests the YOUR_MODULE module functionality.
      */
    class YourModuleTestCase extends DrupalWebTestCase {
      public static function getInfo() {
        return array(
          'name' => 'your_module functionality',
          'description' => 'Test your_module settings.',
          'group' => 'Your Module'
        );
      }

      /**
       * Implementaion of setUp().
       */
      function setUp() {
        parent::setUp('your_module');
      }

      /**
       * Implementaion of tearDown().
       */
      function tearDown() {
        parent::tearDown();
      }

      // Function for condition if($msg == "Hello World") {
      function testYour_module_say_hello_if_0() {
        
      }
     }

5) After skeleton generation you can write your test logic within function
   which is generated based on conditions in YOUR_MODULE.

   Logic can be like:
   // Function for condition if($msg == "Hello World") {
   function testYour_module_say_hello_if_0($mgs) {
     // Pass $msg as function parameter and check if $msg is equal to "Hello World"
     $this->assertEqual($mgs, "Hello World", t('String values are equal'), 'Your Module');
   }

6) Flush all cache and run test from
   admin/config/development/testing.

  Credits
====================================
pgautam (sn_coder_plus): https://drupal.org/user/884876
