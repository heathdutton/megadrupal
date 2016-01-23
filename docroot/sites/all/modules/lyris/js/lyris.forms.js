/**
 * @file
 * Form specific behaviours.
 */

(function ($) {
  /**
   * Determine if the fieldset has the class indicating required fields and
   * return the proper value.
   */
  $.fn.hasRequired = function() {
    if ($(this).hasClass('required-fields')) {
      return '<p class="form-required">' + Drupal.t('Contains required fields.') + '</p><br />';
    }
    else {
      return '';
    }
  }
})(jQuery);
