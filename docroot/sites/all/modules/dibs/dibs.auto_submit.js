/**
 * @file
 * Handles autosubmitting the DIBS payment form.
 */
(function ($) {
  Drupal.behaviors.dibsAutoSubmit = {
    attach: function(context, settings) {
      $("#dibs-frontend-redirect-form").submit();
    }
  }
})(jQuery);
