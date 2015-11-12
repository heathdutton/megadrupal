/**
 * Defines the beautytips settings for form errors
 */
(function ($) {
  Drupal.behaviors.btFormErrors = {
    attach: function(context, settings) {
      // Fix for drupal attach behaviors in case the plugin is not attached.
      if (typeof(jQuery.bt) == 'undefined' && jQuery.bt == null){
        return;
      }

      // Sets beautytips with bt_form_erros module custom settings for all form elements
      $('input.error, textarea.error, select.error', context).each(function(){
        var element = $(this);

        element.bt($(this).attr('data-error-text'), Drupal.settings.bt_form_errors);
        element.btOn();
      });
    }
  };
})(jQuery);
