/**
 * @file
 * Required functions to use the Pirobox Limit administration.
 */

(function ($) {
  Drupal.behaviors.initPlAdmin = {
    attach: function(context, settings) {
      togggleLoses();
      // Drupals #states can not hide/unhide on textareas
      // the filter wrapper fieldset and the description.
      $('#edit-pirobox-limit-extra-style').click(function() {
        togggleLoses();
      });
      function togggleLoses() {
        if ($('#edit-pirobox-limit-extra-style').is(':checked')) {
          $('#edit-pirobox-limit-extra-caption-format').parent().show();
          $('#edit-pirobox-limit-extra-style-description').hide();
        }
        else {
          $('#edit-pirobox-limit-extra-caption-format').parent().hide();
          $('#edit-pirobox-limit-extra-style-description').show();
        }
      }
    }
  };
})(jQuery);
