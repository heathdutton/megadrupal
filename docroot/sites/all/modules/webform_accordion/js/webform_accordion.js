/**
 * @file
 * Enables the Accordion display on webforms.
 */

(function ($) {
  Drupal.behaviors.webform_accordion = {
    attach: function(context) {

      for (var id in Drupal.settings.webform_accordion) {
        var options = {}
        for (var option_name in Drupal.settings.webform_accordion[id]) {
          options[option_name] = Drupal.settings.webform_accordion[id][option_name];
        }
        $('#' + id).accordion(options);
      }
    }
  }
})(jQuery);
