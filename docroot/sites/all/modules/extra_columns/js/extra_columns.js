/**
 * @file
 * Primary js file
 *
 * Handles all js function in this module
 */

(function ($) {
  Drupal.behaviors.extra_columns = {
    attach: function (context, settings) {
      var ec_form_updated = false;

      // Create dialog box for selecting columns.
      var ec_dialog = $('<div>', {
        'class': 'extra-column-dialog'
      }).appendTo($('body'));

      ec_dialog.append($('<div>', {'class': 'dialog-content'}));

      var ec_add_href = $('#extra_columns_add').attr('href');

      $('#extra_columns_add').attr('href', ec_add_href + '?ajax=1');
      $('#extra_columns_add').click(function () {
        ec_dialog.find('.dialog-content').load($(this).attr('href'), function () {
          ec_dialog.dialog({
            autoOpen: true,
            closeOnEscape: true,
            closeText: 'Close',
            width: '85%',
            modal: true,
            close: function () {
              if (ec_form_updated) {
                window.location.reload();
              }
            }
          });
        });

        return false;
      });

      // Ghetto ajaxifies select column form.
      $('#extra-columns-fields-form').live('submit', function () {
        var $form = $(this);
        var url = $(this).attr('action');
        var inputs = $(this).find('input');
        var params = {};

        //get all post variables
        $.each(inputs, function (i, e) {
          var $e = $(e);

          if ($e.is(':checkbox')) {
            if ($e.is(':checked')) {
              params[$(e).attr('name')] = $(e).val();
            }
          } else {
            params[$(e).attr('name')] = $(e).val();
          }
        });

        // Send to drupal.
        $.post(url, params, function (data) {
          var $new_form = $(data);

          //close my eyes, assume everything's ok
          $new_form.find('input:submit').after(
            $('<div>', {'class': 'status ok'}).width(24).height(24)
          )

          $form.replaceWith($new_form);

          ec_form_updated = true;
        });

        return false;
      });
    }
  }
})(jQuery)
