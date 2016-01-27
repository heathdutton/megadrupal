(function ($) {
/**
 * States.
 */
Drupal.tests.testswarm_forms_tableselect = {
  getInfo: function() {
    return {
      name: 'Tableselect',
      description: 'Tests for Tableselect.',
      group: 'System'
    };
  },
  tests: {
    select_all: function ($, Drupal) {
      return function() {
        expect(2);

        // Machine name should be hidden on page load
        ok($('table.table-select-processed .select-all input:visible').length, Drupal.t('"Select all" checkbox found'));

        // Check the select-all checkbox
        $('table.table-select-processed .select-all input:visible').first().attr('checked', 'checked').trigger('click').attr('checked', 'checked');
        var all_checked = true;
        $('table.table-select-processed tr input[type=checkbox]:visible').each(function() {
          if (!$(this).attr('checked') && $(this).attr('checked') != 'checked') {
            all_checked = false;
          }
        });
        ok(all_checked, Drupal.t('All checkboxes in the table are checked.'));
      }
    },
    deselect_all: function ($, Drupal) {
      return function() {
        expect(1);

        // Uncheck the select-all checkbox
        $('.select-all input:visible').first().removeAttr('checked').trigger('click');
        var all_checked = false;
        $('table.table-select-processed tr input[type=checkbox]:visible').each(function() {
          if ($(this).attr('checked') || $(this).attr('checked') == 'checked') {
            all_checked = false;
          }
        });
        ok(!all_checked, Drupal.t('No checkboxes in the table are checked'));
      }
    }
  }
};
})(jQuery);
