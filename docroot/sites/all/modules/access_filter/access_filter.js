(function($) {
  Drupal.behaviors.access_filter = {
    attach: function(context) {
      $('#edit-deny-action-settings-type').change(function() {
        var value = $(this).val();
        var elem_message = $('#edit-deny-action-settings-error-message-value').parents('.text-format-wrapper');
        var elem_redirect = $('#edit-deny-action-settings-redirect-destination').parents('.form-item');
        if (value == '403' || value == '404') {
          elem_message.show();
          elem_redirect.hide();
        }
        else {
          elem_message.hide();
          elem_redirect.show();
        }
      }).trigger('change');
    }
  }
})(jQuery);
