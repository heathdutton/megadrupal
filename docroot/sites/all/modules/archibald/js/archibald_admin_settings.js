(function ($) {

  Drupal.behaviors.archibald_admin_settings_form = {
    attach: function (context, settings) {
      archibald_admin_settings_handle_toggle_emails('#edit-enable-email-new-unavailable');
      archibald_admin_settings_handle_toggle_emails('#edit-enable-email-status2final');
      archibald_admin_settings_handle_toggle_emails('#edit-enable-email-status2draft');
      archibald_admin_settings_handle_toggle_emails('#edit-enable-email-set-responsible');
    }
  };

  function archibald_admin_settings_handle_toggle_emails(id) {
    $(id).unbind('change').bind('change', function(){
      archibald_admin_settings_toggle_emails(id);
    });
    archibald_admin_settings_toggle_emails(id);
  }
  function archibald_admin_settings_toggle_emails(id) {
    var elm = $('> * :not(.fieldset-description, .fieldset-description *, .form-type-checkbox, .form-type-checkbox *)', $(id).parent().parent());
    if ($(id).attr('checked')) {
      $(elm).show();
    }
    else {
      $(elm).hide();
    }
  }
})(jQuery);



