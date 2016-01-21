/**
 * @file
 * Helper code to help iframe escape.
 *
 * @see http://cgit.drupalcode.org/commerce_paypal/tree/modules/payflow/commerce_payflow.js?h=7.x-2.x
 */

(function($) {
    // Escapes from an iframe if the completion page is displayed within an iframe.
    Drupal.behaviors.commerceBluepayEscapeIframe = {
        attach: function (context, settings) {
            if (top !== self) {
                window.parent.location.href = window.location.href;
            }
        }
    }
})(jQuery);
