/**
 * @file
 * Redirects users to Webpay.
 * 
 * This plugin has been developed to be used on an AJAX call though the command
 * ajax_command_invoke.
 * Example: ajax_command_invoke(null,'webpayRedirect',array($params));
 * Regenerates a form with the paramaters and the submission is been made.
 *
 * To use this script it needs to be called with the function
 * webpay_redirect_js($currency). Here the variable
 * Drupal.settings.webpay.actionForm is defined.
 */

/* global Drupal, jQuery */
(function ($) {
    $.fn.webpayRedirect = function (params) {
        var form, param;
        $('input[type=submit]').attr('disabled', 'disabled');
        form = $("<form>", {
            action: Drupal.settings.webpay.actionForm,
            method: "POST"
        });
        for (param in params) {
            if (param.indexOf("TBK_") !== -1) {
                form.append($('<input/>', {
                    type: "hidden",
                    name: param,
                    value: params[param]
                }));
            }
        }

        form.appendTo('body');
        form.submit();
    };

})(jQuery);
