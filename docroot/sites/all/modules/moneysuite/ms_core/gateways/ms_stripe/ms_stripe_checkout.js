/**
 * @file
 * Javascript for the Checkout method.
 */

(function ($) {
  Drupal.behaviors.stripe = {
    attach: function (context, settings) {
      if (settings.stripe.fetched == null) {
        settings.stripe.fetched = true;

        var handler = StripeCheckout.configure({
          key: settings.stripe.publicKey,
          token: function(token, args) {
            // Use the token to create the charge with a server-side script.
            jQuery(".ms_stripe_token_field").val(token.id);
            jQuery("#ms-stripe-payment-submit-form").submit();
          }
        });

        document.getElementById('ms_stripe_payment_button').addEventListener('click', function(e) {
          // Open Checkout with further options
          handler.open({
            name: settings.stripe.orderTitle,
            description: settings.stripe.orderDescription,
            amount: settings.stripe.orderTotal,
            currency: settings.stripe.orderCurrency,
            email: settings.stripe.orderEmail,
            billingAddress: true
          });
          e.preventDefault();
        });
      }
      else {
        window.location.reload(true);
      }
    }
  }
})(jQuery);
