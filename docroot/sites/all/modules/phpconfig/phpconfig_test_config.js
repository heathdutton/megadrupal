(function ($) {
  Drupal.behaviors.testPhpConfigBehavior = {
    attach:function (context) {
      $('#phpconfig-newform').submit(function() {
        var item = $('#edit-item').val();
        var value = $('#edit-value').val();
        var status = $('#edit-status').attr('checked');
        error_flag = true;
        // Test the new phpconfig via ajax.
        if ((typeof status == 'undefined' || status == true) && item.length != 0 && value.length != 0 && button_pressed == 'edit-submit') {
          $.ajax({
            url: Drupal.settings.phpconfig_test.ajaxUrl,
            data: {item: item, value: value, phpconfig_tok: Drupal.settings.phpconfig_test.phpconfig_tok},
            async: false,
            dataType: 'json',
            error: function(jqXHR, textStatus, errorThrown) {
              Drupal.ajaxFormSetError();
              error_flag = false;
            },
            success: function(response) {
                if (response == null) {
                  Drupal.ajaxFormSetError();
                  error_flag = false;
                }
            }
          });
          return error_flag;
        }
      });
      var button_pressed;
      $('#phpconfig-newform .form-submit').click(function() {
          button_pressed = $(this).attr('id');
      })
    }
  };

  Drupal.ajaxFormSetError = function() {
      // Remove any existing error message.
      $('.messages.error').remove();
      $('#phpconfig-newform').prepend('<div class="messages error">' + Drupal.settings.phpconfig_test.msg + '</div>');
      $('#edit-value').addClass('error');
      $('#edit-item').removeClass('error');
  }
})(jQuery);
