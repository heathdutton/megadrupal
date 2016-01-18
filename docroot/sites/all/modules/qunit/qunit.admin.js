(function($) {

/**
 * Provide a Drupal-specific wrapper for the QUnit JavaScript test framework.
 */
Drupal.tests = Drupal.tests || {};

Drupal.behaviors.runTests = {
  attach: function(context, settings) {
    var index;
    var loaded = 0;
    for (index in Drupal.tests) {
      var testCase = Drupal.tests[index];
      var info = testCase.getInfo();
      module(info.group, testCase);
      test(info.name, testCase.test);
    }
  }
};

})(jQuery);
