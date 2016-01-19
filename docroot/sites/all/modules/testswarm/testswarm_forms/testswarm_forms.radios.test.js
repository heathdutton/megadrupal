(function ($) {
/**
 * Required radio and AJAX.
 */
Drupal.tests.testswarm_forms_radios = {
  getInfo: function() {
    return {
      name: 'Required radio and AJAX',
      description: 'Tests for Required radio and AJAX.',
      group: 'System'
    };
  },
  tests: {
    empty_radio: function ($, Drupal) {
      return function() {
        expect(1);
        
        // Make sure the radio buttons aren't checked
        equal($('input[name="field_radios[und]"]:checked').length, 0, Drupal.t('No radio is checked'));
        
        // Click on the add more buttom.
        QUnit.stop();
        setTimeout(function(){QUnit.start();}, 2000);
        $('#edit-field-multitext-und-add-more').trigger('mousedown');
      }
    },
    empty_radio_message: function ($, Drupal) {
      return function() {
        expect(1);
        
        // Check for messages.error
        equal($('div.messages.error:visible').length, 0, Drupal.t('"An illegal choice has been detected. Please contact the site administrator." is not displayed'));
      }
    }
  }
};
})(jQuery);