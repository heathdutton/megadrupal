(function ($) {
/**
 * Tests autocomplete.
 */
Drupal.tests.autocomplete = {
  getInfo: function() {
    return {
      name: 'autocomplete',
      description: 'Tests for autocomplete.',
      group: 'System',
      useSimulate: true
    };
  },
  tests: {
    existing_item: function ($, Drupal) {
      return function() {
        expect(6);
        var delay = 1000;

        $('#edit-text1').val('aa').trigger('keyup');
        stop();
        setTimeout(function() {
          ok($('#autocomplete:visible'), 'autocomplete list visible');
          ok($('#autocomplete li').first().find('aaa'), 'aaa found');
          ok($('#autocomplete li').first().next().find('aaabbb'), 'aaabbb found');
          ok($('#autocomplete li').first().find('ccc').length == 0, 'ccc not found');

          $("#edit-text1").simulate("keydown", { keyCode: 40 });
          $("#edit-text1").simulate("keyup", { keyCode: 13 });

          setTimeout(function() {
            ok($('#edit-text1').val() == 'aaa', 'aaa selected');
            ok($('#autocomplete').length == 0, 'autocomplete not visible');
            start();
          }, delay);
        }, delay);
      }
    },
    non_existing_item: function ($, Drupal) {
      return function() {
        expect(1);
        var delay = 1000;

        $('#edit-text1').val('xx').trigger('keyup');
        stop();
        setTimeout(function() {
          ok($('#autocomplete.length').length == 0, 'autocomplete not visible');
          start();
        }, delay);
      }
    }
  }
};
})(jQuery);