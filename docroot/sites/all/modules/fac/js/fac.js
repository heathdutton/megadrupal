/**
 * @file
 * Fast Autocomplete scripts.
 */

(function ($) {
  /**
   * Add the Fast Autocomplete functionality.
   */
  Drupal.behaviors.fac = {
    attach: function(context, settings) {
      $(Drupal.settings.fac.inputSelectors).once('fac', function() {
        $(this).fastAutocomplete(Drupal.settings.fac);
      });
    }
  };

}(jQuery));
