/**
 * @file
 * To add input method support to the editable fields of a web page.
 */

(function($) {
  Drupal.behaviors.jqueryImeDrupal = {
    attach: function(context, settings) {
      var flb_uid = Drupal.settings.ime.field_ids;
      var flb_path = Drupal.settings.ime.jqueryImePath;
      $(flb_uid).ime({imePath: flb_path});
    }
  };
})(jQuery);
