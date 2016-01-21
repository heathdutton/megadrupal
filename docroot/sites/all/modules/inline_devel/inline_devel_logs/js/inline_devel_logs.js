/**
 * @file
 * Js file for Inline Devel logs.
 */
(function($) {
  Drupal.behaviors.showHideCodes = {
    attach: function() {
      $("#edit-history").click(function() {
        $("#history").toggle('fast');
      });
    }
  }
})(jQuery);
