/**
 * @file
 * Javascript Handler for the YellowPay Return.
 * Compatible with ShopBuilder.me eCommerce platform
 * Author Eweev S.A.R.L (www.eweev.com)
 * Sponsor YellowPay (www.yellowpay.co)
 */

(function($) {
  Drupal.behaviors.commerce_yellowpay = {
    attach: function(context, settings) {
      function invoiceListener(event) {
        // For additional security, confirm the message originated from the
        // embedded invoice.
        if (!/\.yellowpay\.co$/.test(event.origin)) {
          alert(Drupal.t('Received message from unexpected domain: @origin', {'@origin': event.origin}));
          return;
        }
        // Handle the invoice status update.
        if (event.data == 'authorizing' || event.data == 'paid') {
          // We redirect the checekout step to the complete pane if the payment is being authorized.
          window.location.href = settings['yellowPay']['return-url'];
        } else if (event.data == 'expired') {
          // We inform the user that the payment expired and take him/her back to the previous step.
          window.location.href = $("#cancel_yellowpay_payment").attr('href');
        }
      }
      // Attach the message listener.
      if (window.addEventListener) {
        addEventListener("message", invoiceListener, false)
      } else {
        attachEvent("onmessage", invoiceListener)
      }
    }
  };
}(jQuery));
