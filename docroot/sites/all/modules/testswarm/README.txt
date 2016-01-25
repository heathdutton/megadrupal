Test Swarm
----------

How to write tests for your own code
------------------------------------

1/ Implement hook_testswarm_tests

/**
 * Implements hook_testswarm_tests().
 */
function MY_MODULE_testswarm_tests() {
  return array(
    'MY_MODULE_test' => array(
      'js' => array(
        drupal_get_path('module', 'MY_MODULE') . '/MY_MODULE.test.js' => array(),
      ),
      'path' => '<path to test>',
      'permissions' => '<permission needed to access this test>',
    ),
  );
}

2/ Clear your cache so the hook is detected

3/ Create the file MY_MODULE.test.js

(function ($) {
  Drupal.behaviors.MY_MODULE_test = {
    attach: function (context) {
      module("MY MODULE test group", {
        setup: function() {
          // Optional
        },
        teardown: function() {
          // Optional
        }
      });    
      test(Drupal.t('Name of the test'), function() {
        expect(1); // <-- VERY IMPORTANT, must be the number of tests
        
        // Do some jQuery magic

        // Test it
        equal(1, 1, Drupal.t('This is always true ;p'));
      });
    }
  };
})(jQuery);

4/ Go to testswarm-browser-tests and the test will run

5/ To debug go to /testswarm-run-a-test/MY_MODULE_test?debug=on