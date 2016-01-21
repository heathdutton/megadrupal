/**
 * @file
 * commerce_xls_import.js
 *
 *  Custom JavaScript for the Commerce xls import.
 */

var timer;
(function ($) {
    Drupal.behaviors.commerce_xls_import = {
        attach: function (context, settings) {
            $('#edit-import-stop', context).once('commerce_xls_import', function () {

                $('#edit-import-stop').click(function () {
                    $('#import_status').val(0);
                    clearTimeout(timer);
                });

                if ($('input[name="import_status"]').val() == 1) {
                    // Start counting after 2 seconds;
                    timer = setInterval(function () {
                        start_updates()
                    }, 1000);
                }
            });
        }
    };

  function start_updates() {
    periodic_update_import_status();
  }

  function periodic_update_import_status() {
    $.ajax({
      url: Drupal.settings.basePath + "?q=admin/commerce/products/import_commerce/get_import_status",
      type: "GET",
      dataType: "json",
      data: {},
      success: function(data){
        var message_box = $('#import_status_messages');
        message_box.css('display', 'block');
        message_box.html(data.message);
        if (data.status == 1) {
          location.reload();
        }
      }

    });
  }
})(jQuery);
