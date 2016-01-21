
/**
 * @file
 * This file set a new plugin for jquery for redirect the user to Khipu.
 */

(function ($) {
    $.fn.khipuRedirect = function (params) {
        var form, param;
        // Disable all submit input type.
        $('input[type=submit]').attr('disabled', 'disabled');
        form = $("<form>", {
            action: Drupal.settings.khipu.actionForm,
            method: "POST"
        });
        for (param in params) {
          // Attach params on form.
          form.append($('<input/>', {
              type: "hidden",
              name: param,
              value: params[param]
          }));
        }
        form.append($('<input/>', {
          type: "submit",
          value: "Go",
        }));
        // Attach it on body.
        form.appendTo('body');
        // Hide it.
        form.hide();
        // Submit it.
        form.submit();
    };

})(jQuery);
