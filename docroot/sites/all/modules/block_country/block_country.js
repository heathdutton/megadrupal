/**
 * @file
 * Settings restricted values on country fieldset.
 */

(function ($) {
  "use strict";
  // Define a Drupal behaviour with a custom name.
  Drupal.behaviors.blockCountrySettings = {
    attach: function (context) {

      $('fieldset#edit-block-country', context).drupalSetSummary(function (context) {
        // Check selected country.
        if (!$('#edit-country-code', context).val()) {
          return Drupal.t('Not restricted');
        }
        else {
          return Drupal.t('Restricted to Specific Country');
        }
      });
    }
  };
})(jQuery);
