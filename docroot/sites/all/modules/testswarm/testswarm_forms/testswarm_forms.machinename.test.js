(function ($) {
/**
 * Machine name - empty.
 */
Drupal.tests.machinename_empty = {
  getInfo: function() {
    return {
      name: 'Machine name - empty',
      description: 'Tests for Machine name - empty.',
      group: 'System'
    };
  },
  tests: {
    machinename_empty_value: function ($, Drupal) {
      return function() {
        expect(3);

        // Machine name should be hidden on page load
        ok($('#edit-name-machine-name-suffix:hidden').length, Drupal.t('Machine name is hidden.'));
        
        // Enter some text, machine name should be visible and contain the right value
        $('#edit-name').val('test').trigger('keyup');
        ok($('#edit-name-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        ok($('#edit-name-machine-name-suffix .machine-name-value').text() == 'test', Drupal.t('Machine name contains the right value.'));
      }
    },
    machinename_empty_maxlength: function ($, Drupal) {
      return function() {
        expect(2);

        // Enter some text, machine name should be visible and contain the right value
        $('#edit-name').val('abcdefghijklmnopqrstuvwxyz').trigger('keyup');
        ok($('#edit-name-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        ok($('#edit-name-machine-name-suffix .machine-name-value').text() == 'abcdefghij', Drupal.t('Machine name is truncated to the right value.'));
      }
    },
    machinename_empty_case: function ($, Drupal) {
      return function() {
        expect(2);

        // Enter some text, machine name should be visible and contain the right value
        $('#edit-name').val('ABCDEFGHIJ').trigger('keyup');
        ok($('#edit-name-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        ok($('#edit-name-machine-name-suffix .machine-name-value').text() == 'abcdefghij', Drupal.t('Machine name is converted to lowercase.'));
      }
    },
    machinename_empty_edit: function ($, Drupal) {
      return function() {
        expect(2);

        // Enter some text
        $('#edit-name').val('ok').trigger('keyup');
        
        // Click the edit link
        $('#edit-name-machine-name-suffix a').trigger('click');
        ok($('#edit-type:visible').length, Drupal.t('Machine name edit box is visible.'));
        
        // Enter some text, machine name should not change
        $('#edit-name').val('this is a another test').trigger('keyup');
        ok($('#edit-name-machine-name-suffix .machine-name-value').text() == 'ok', Drupal.t('Machine name contains the right value.'));
      }
    }
  }
};


/**
 * Machine name - edit.
 */
Drupal.tests.machinename_filled = {
  getInfo: function() {
    return {
      name: 'Machine name - edit',
      description: 'Tests for Machine name - edit.',
      group: 'System'
    };
  },
  tests: {
    machinename_filled_value: function ($, Drupal) {
      return function() {
        expect(3);

        // Machine name should be visible on page load
        ok($('#edit-second-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        
        // Enter some text, machine name should be visible and contain the right value
        $('#edit-second').val('test').trigger('keyup');
        ok($('#edit-second-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        ok($('#edit-second-machine-name-suffix .machine-name-value').text() == 'test', Drupal.t('Machine name contains the right value.'));
      }
    },
    machinename_filled_maxlength: function ($, Drupal) {
      return function() {
        expect(2);

        // Enter some text, machine name should be visible and contain the right value
        $('#edit-second').val('abcdefghijklmnopqrstuvwxyz').trigger('keyup');
        ok($('#edit-second-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        ok($('#edit-second-machine-name-suffix .machine-name-value').text() == 'abcdefghij', Drupal.t('Machine name is truncated to the right value.'));
      }
    },
    machinename_filled_case: function ($, Drupal) {
      return function() {
        expect(2);

        // Enter some text, machine name should be visible and contain the right value
        $('#edit-second').val('ABCDEFGHIJ').trigger('keyup');
        ok($('#edit-second-machine-name-suffix:visible').length, Drupal.t('Machine name is visible.'));
        ok($('#edit-second-machine-name-suffix .machine-name-value').text() == 'abcdefghij', Drupal.t('Machine name is converted to lowercase.'));
      }
    },
    machinename_filled_edit: function ($, Drupal) {
      return function() {
        expect(2);

        // Enter some text
        $('#edit-second').val('ok').trigger('keyup');
        
        // Click the edit link
        $('#edit-second-machine-name-suffix a').trigger('click');
        ok($('#edit-type:visible').length, Drupal.t('Machine name edit box is visible.'));
        
        // Enter some text, machine name should not change
        $('#edit-second').val('this is a another test').trigger('keyup');
        ok($('#edit-second-machine-name-suffix .machine-name-value').text() == 'ok', Drupal.t('Machine name contains the right value.'));
      }
    }
  }
};
})(jQuery);