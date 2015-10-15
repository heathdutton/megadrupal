/**
 * @file
 * Javascript for the Checkout method.
 */

(function ($) {
  Drupal.behaviors.stripe = {
    attach: function (context, settings) {
      if (settings.stripe.fetched == null) {
        settings.stripe.fetched = true;

        var membersify_checkout_handler = StripeCheckout.configure({
          key: settings.stripe.publicKey,
          token: function(token, args) {
            // Save the token and submit the form.
            jQuery(".membersify_payment_token_field").val(token.id);
            jQuery("#membersify-payment-button-form").submit();
          }
        });

        jQuery('#membersify_payment_button').bind('click', function(e) {
          // Open Checkout with further options.
            membersify_checkout_handler.open({
            name: settings.stripe.orderTitle,
            description: settings.stripe.orderDescription,
            amount: settings.stripe.orderTotal,
            currency: settings.stripe.orderCurrency,
            email: settings.stripe.orderEmail,
            billingAddress: true
          });
          e.preventDefault();
        });

        var membersify_add_card_handler = StripeCheckout.configure({
          key: settings.stripe.publicKey,
          token: function(token, args) {
              // Save the token and submit the form.
              jQuery(".membersify_add_card_token_field").val(token.id);
              jQuery("#membersify-add-card-button-form").submit();
          }
        });

        jQuery('#membersify_add_card_button').bind('click', function(e) {
          // Open Checkout with further options.
            membersify_add_card_handler.open({
              email: settings.stripe.email,
              panelLabel: settings.stripe.label,
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
