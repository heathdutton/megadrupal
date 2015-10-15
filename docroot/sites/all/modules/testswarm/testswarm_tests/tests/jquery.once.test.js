(function($, Drupal) {

/**
 * Tests the jQuery Once plugin.
 */
Drupal.tests.once = {
  getInfo: function() {
    return {
      name: 'jQuery Once',
      description: 'Tests for the jQuery Once plugin.',
      group: 'System'
    };
  },
  tests: { 
    createonce: function ($, Drupal) {
      return function() {
        expect(4);

        var html = '<span>Hello</span>';
        var jqueryhtml = $(html);
        $('#page-title').append(jqueryhtml);

        // Test One
        jqueryhtml.once('testone', function() {
          ok(true, Drupal.t('Once function is executed fine.'));
        });
        jqueryhtml.once('testone', function() {
          ok(false, Drupal.t('Once function is executed twice.'));
        });

        // Test Two
        jqueryhtml.once('testtwo', function() {
          ok(true, Drupal.t('Once function is executed fine one different tests.'));
        });

        // Test Three
        jqueryhtml.once('newclassfortestthree').addClass('testthreecomplete');
        ok(jqueryhtml.hasClass('testthreecomplete'), Drupal.t('Once each function is called.'));

        // Test Four
        jqueryhtml.once('newclassfortestthree').addClass('failure');
        equal(jqueryhtml.hasClass('failure'), false, Drupal.t('Once each function is called multiple times rather then once.'));
      }
    }
  }
};

})(jQuery, Drupal);
