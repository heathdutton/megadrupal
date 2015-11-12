/**
 * @file
 * JavaScript to show/hide secret field's content.
 */

(function ($) {
  Drupal.behaviors.contextIOAdminShowHideCheckbox = {
    attach: function (context, settings) {
      // Drupal misses the 'for' attribute for the label, so add it here...
      $('.form-item-contextio-secret .field-suffix .form-type-checkbox label').attr('for', $('.form-item-contextio-secret .field-suffix .form-type-checkbox input').attr('id'));

      $('.form-item-contextio-secret .field-suffix .form-type-checkbox').click(function(event) {
        var input = $(this).find('input.form-checkbox');
        var input_value = $('.form-item-contextio-secret #edit-contextio-secret').val();

        if ($(input).is(':checked')) {
          $('.form-item-contextio-secret #edit-contextio-secret').replaceWith('<input value="' + input_value + '" type="text" id="edit-contextio-secret" name="contextio_secret" size="60" maxlength="128" class="form-text required">');
        }
        else {
          $('.form-item-contextio-secret #edit-contextio-secret').replaceWith('<input value="' + input_value + '" type="password" id="edit-contextio-secret" name="contextio_secret" size="60" maxlength="128" class="form-text required">');
        }
      });
    }
  }
})(jQuery);
