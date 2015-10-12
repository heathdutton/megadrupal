(function ($) {


  Drupal.behaviors.RESTSettings = {
    attach: function (context, settings) {
      $('#redmine-sync-settings-rest-api-form', context).once('rest-settings', function () {
        var el_form = $(this),
            el_rest_auth_mode = $('#edit-rest-auth-mode', el_form),
            el_admin_key_wr = $('.form-item-admin-key', el_form);
        el_rest_auth_mode.change(function(){
          if (el_rest_auth_mode.attr('value') != 0) {
            el_admin_key_wr.hide();
          } else {
            el_admin_key_wr.show();
          }
        });
        el_rest_auth_mode.change();
      });
    }
  };


  Drupal.behaviors.DatePicker = {
    attach: function (context, settings) {
      $('.datepicker', context).once('datepicker', function () {
        $(this).datepicker({dateFormat: 'yy-mm-dd'});
        $('<span class="datepicker-icon"></span>').insertAfter($(this));
      });
    }
  }


})(jQuery);