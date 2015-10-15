/**
 * @file
 * Field level implementation of ETS Hosted Payments.
 */

(function($) {
  var sessionSelector = '#commerce-ets-emoney-session-id';
  var checkoutFormSelector = 'form[id^=commerce-checkout]';

  Drupal.behaviors.commerceETS = {
    attach: function(context, settings) {

      if (!window.ETSPayment) {
        // Execute this function until ETSPayment is available.
        var checkETSPayment = setInterval(function () {
          if (window.ETSPayment) {
            clearInterval(checkETSPayment);
            commerceEtsAttachHandlers();
            $('input.checkout-continue').removeAttr('disabled');
          }
          else {
            $('input.checkout-continue').attr('disabled', 'disabled');
          }
        }, 100);
      }
      else {
        commerceEtsAttachHandlers();
      }

      $(checkoutFormSelector).submit(function(event) {
        if (!$(this).hasClass('ets-processed') && window.ETSPayment && $(sessionSelector).length > 0) {
          $(this).addClass('ets-processed');
          window.ETSPayment.submitETSPayment();
          event.preventDefault();
          return false;
        }
      });
    }
  };

  // Add response handlers to the Payment.
  function commerceEtsAttachHandlers() {
    if (window.ETSPayment) {
      window.ETSPayment.useHiddenSDK($(sessionSelector).data('commerce-ets-sid'));
      window.ETSPayment.addResponseHandler("validation", commerceETSValidateFields);
      window.ETSPayment.addResponseHandler("error", commerceETSPaymentError);
      window.ETSPayment.addResponseHandler("success", commerceETSPaymentSuccess);
    }
  }

  // Error response handler, not sure what to do here.
  function commerceETSPaymentError() {
    $(checkoutFormSelector).trigger('submit');
  }

  // Trigger the submission in case of success.
  function commerceETSPaymentSuccess(paymentObj) {
    var checkoutForm = $(checkoutFormSelector);
    $('#payment-details input:text', checkoutForm).val('');
    $('.commerce-ets-emoney-transaction-id', checkoutForm).val(paymentObj.transactions.id);
    $(checkoutForm).trigger('submit');
  }

  // Validate handler for the credit card fields.
  function commerceETSValidateFields(errors) {
    if (errors) {
      $('#payment-details .messages').remove();
      var htmlError = '<div class="messages error"><ul>';
      htmlError += '<li>' + Drupal.checkPlain(errors) + '</li>';
      htmlError += '</ul></div>';
      $('#payment-details').prepend(htmlError);
      $('span.checkout-processing').addClass('element-invisible');
      var continueButtonsSelector = 'input.checkout-continue';

      // Remove the additional continue button added by commerce_checkout.
      if ($(continueButtonsSelector).length > 1) {
        $(continueButtonsSelector).get(1).remove();
        $(continueButtonsSelector).show();
      }
      $(checkoutFormSelector).removeClass('ets-processed');
    }
  }

})(jQuery);

