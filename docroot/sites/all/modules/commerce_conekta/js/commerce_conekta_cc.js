/**
* Handles tokenization in the checkout
* Token sent to server which handles the transaction in charge.php
*/
(function ($) {

Drupal.behaviors.conekta_cc_behaviors = {
  attach : function(context, settings) {
    var that = this;

    $('form[class*="commerce-checkout-form"]').once('conekta', function() {
      // Set Conekta public key
      Conekta.setPublishableKey(Drupal.settings.commerce_conekta_cc.public_key);
      $form = $(this);

      $('input[type="submit"]').bind('click', function(event) {
        $form.clk = this;
      });

      // Hide Conekta error messages when payment method is changed
      $('input[type="radio"]', 'fieldset.commerce_payment').bind('click', function(event) {
        $('.conekta.messages.error').empty().hide();
      });

      $form.bind('submit', function(event) {
        // Empty error messages
        $('.conekta.messages.error').empty().hide();
        // Display the progress indicator
        $('span.checkout-processing').removeClass('element-invisible');

        // Only react when submit is triggered through continue button so cancel
        // button behave correctly.
        if ($form.clk.id != 'edit-continue') {
          return true;
        }

        // Find which payment method is selected
        var method = $(':checked', 'fieldset.commerce_payment').val();

        // Only tokenize when used payment method is conekta credit card
        if (method !== undefined && method.indexOf("conekta_cc") === -1) {
          return true;
        }

        // Prevent default behavior so we take control when form submission happens
        event.preventDefault();
        $form.find('input[type="submit"]').attr("disabled", "disabled");
        Conekta.token.create($form, that.conektaSuccessResponseHandler, that.conektaErrorResponseHandler);
      });
    });
  },

  conektaSuccessResponseHandler : function(response) {
    $('input[name="commerce_payment[payment_details][token]"]').val(response.id);
    $('form[class*="commerce-checkout-form"]').get(0).submit();
  },

  conektaErrorResponseHandler : function(response) {
    var $form = $('form[class*="commerce-checkout-form"]');
    var message = Drupal.t(response.message_to_purchaser);

    if ($('.conekta.messages.error').length) {
      $('.conekta.messages.error').html(message);
    }
    else {
      $('fieldset.commerce_payment', $form).append('<div class="conekta messages error">' + message + '</div>');
    }

    $('.conekta.messages.error').fadeIn(2000);
    $form.find('input[type="submit"]').removeAttr("disabled");
    // Hide progress indicator
    $('span.checkout-processing').addClass('element-invisible');
  },
};

})(jQuery);
