/**
 * @file
 * Form default button Javascript implementation.
 */

(function ($) {

Drupal.behaviors.form_default_button = {
  attach: function (context, settings) {
    // For every Form default button setting.
    $.each(settings.form_default_button, function(form_id, setting) {
      // Bind the click event for the hidden button.
      var $form = $('form#' + form_id, context);
      $form.find('input#' + setting.button_id).removeAttr('disabled').bind('click', function(e) {
        e.preventDefault();
        switch (setting.action) {
          case 'none':
            // Do nothing. We already stopped the default form behavior.
            break;

          case 'click':
            // Trigger action based on JS settings.
            $form.find(setting.target).first().trigger(setting.action);
            break;
        }
      });
    });
  }
}

})(jQuery);
