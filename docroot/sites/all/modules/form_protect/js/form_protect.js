/**
 * @file
 * Restores the form action.
 */
(function ($) {
  Drupal.behaviors.protect = {
    attach: function (context) {
      $.each(Drupal.settings.formProtect, function (formId, formAction) {
        $('form#' + formId).attr('action', formAction);
      });
    }
  };
})(jQuery);
