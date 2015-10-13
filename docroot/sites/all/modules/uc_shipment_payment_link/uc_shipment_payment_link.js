/**
 * @file
 * JavaScript helper functions.
 */

(function($) {
  Drupal.UCShipmentPaymentLink = Drupal.UCShipmentPaymentLink || {};

  Drupal.behaviors.UCShipmentPaymentLink = {
    attach: function(context) {
      $("input:radio[name='panes[quotes][quotes][quote_option]']").change(function() {
        if ($(this).attr('checked')) {
          var shipping_method = $(this).val().split("---")[0];
          $("input:radio[name='panes[payment][payment_method]']").each(function() {
            var payment_method = $(this).attr("value");
            var allowed = Drupal.settings.uc_shipment_payment_link.allowed[shipping_method][payment_method];
            if (allowed === undefined || allowed === "1") {
              $(this).attr('disabled', '').parent().show();
            }
            else {
              $(this).attr('disabled', 'disabled').parent().hide();
            }
          });
          $("input:radio[name='panes[payment][payment_method]']:visible:first").click();
        }
      });
    }
  };

})(jQuery);
