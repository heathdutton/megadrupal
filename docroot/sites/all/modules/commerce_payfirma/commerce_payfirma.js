/**
 * @file
 * Performs tokenization on entered Credit Card information 
 * by overriding normal checkout form submit.
 */

(function ($) {

  Drupal.behaviors.commerce_payfirma = {
    attach: function (context, settings) {
      $("#edit-back", context).click(function () {
        $("#edit-back", context).addClass("clicked");
      });
      // Add space every 4 digits in the credit card number.  Allow only digits.
      $("#edit-commerce-payment-payment-details-credit-card-number", context).keyup(function () {
        $(this).val(function (iteration, value) {
          var value = value.replace(/[^\d]/g, '').match(/.{1,4}/g);
          return value ? value.join(' ') : '';
        });
      });

      // Attach a submit handler to the form.
      $("#commerce-checkout-form-" + settings.commerce_payfirma.checkout_page, context).once().submit(function(event) {
        // Stop form from submitting normally.
        if(!$("#edit-back", context).hasClass("clicked") && $('#edit-commerce-payment-payment-method-payfirmacommerce-payment-payfirma', context).is(':checked')) {
          event.preventDefault();
        }
        $('#edit-continue', context).attr('disabled', 'disabled');
        // Form validation.
        var orderId = $('input[name="commerce_payment[payment_details][order_id]"]', context).val();
        var validation = true;
        if(!$("#edit-back", context).hasClass("clicked") && ($.trim($('input[name="commerce_payment[payment_details][credit_card][number]"]', context).val()) == ''
          || $.trim($('#edit-commerce-payment-payment-details-credit-card-code', context).val()) == '')) {
          alert(Drupal.t('Please ensure your card information is correct, and you\'ve entered the security code located on the back of your card.'));
          location.href = settings.basePath + 'checkout/' + orderId + '/' + settings.commerce_payfirma.checkout_page;
          validation = false;
        }
        if(!$("#edit-back", context).hasClass("clicked") && validation) {
          var token = new Payfirma(settings.commerce_payfirma.publicKey, {
              "card_number": $('input[name="commerce_payment[payment_details][credit_card][number]"]', context).val(),
              "card_expiry_month": $('#edit-commerce-payment-payment-details-credit-card-exp-month', context).val(),
              "card_expiry_year": $('#edit-commerce-payment-payment-details-credit-card-exp-year', context).val(),
              "cvv2": $('#edit-commerce-payment-payment-details-credit-card-code', context).val(),
            }, {
              "function": settings.commerce_payfirma.func,
              "amount": $('input[name="commerce_payment[payment_details][amount]"]', context).val(),
              "email": $('input[name="commerce_payment[payment_details][email]"]', context).val(),
              "first_name": $('input[name="commerce_payment[payment_details][first_name]"]', context).val(),
              "last_name": $('input[name="commerce_payment[payment_details][last_name]"]', context).val(),
              "address1": $('input[name="commerce_payment[payment_details][address1]"]', context).val(),
              "address2": $('input[name="commerce_payment[payment_details][address2]"]', context).val(),
              "city": $('input[name="commerce_payment[payment_details][city]"]', context).val(),
              "province": $('input[name="commerce_payment[payment_details][provice]"]', context).val(),
              "country": $('input[name="commerce_payment[payment_details][country]"]', context).val(),
              "postal_code": $('input[name="commerce_payment[payment_details][postal_code]"]', context).val(),
              "company": "",
              "telephone": "",
              "description": "",
              "order_id": $('input[name="commerce_payment[payment_details][order_id]"]', context).val(),
              "invoice_id": $('input[name="commerce_payment[payment_details][invoice_id]"]', context).val(),
              "amount_tax": "",
              "amount_tip": ""
          }, '/commerce_payfirma/handler', payfirmaResult);
        }
      });

      function payfirmaResult(response){
        var obj = JSON.parse(response);
        location.href = settings.basePath + 'checkout/' + obj.order_id;
      }
    }
  };

}(jQuery));
