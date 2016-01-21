/**
 * Contains code for wiring up payment using pin JavaScript API.
 */
(function($) {
  Drupal.behaviors.commercePin = {
    attach: function(context, settings) {
      var pinApi = new Pin.Api(settings.commerce_pin.key, settings.commerce_pin.mode);
      var pinSettings = settings.commerce_pin;

      var $form = $('#commerce-checkout-form-review', context),
        $submitButton = $form.find("#edit-continue"),
        $errorContainer = $form.find('#commerce-pin-errors'),
        $errorList = $errorContainer.find('ul'),
        $errorHeading = $errorContainer.find('strong'),
        $newSubmit = $submitButton.clone(false).attr('id', 'commerce-pin-submit').attr('class', 'form-submit');

      $newSubmit.insertAfter($submitButton);
      $submitButton.hide();
      var $spinner = $('<span class="checkout-processing"></span>');

      // 3. Add a submit handler to the form which calls Pin.js to
      // retrieve a card token, and then add that token to the form and
      // submit the form to our server.
      $newSubmit.once('commerce-pin', function() {
        $(this).click(function(e) {
          e.preventDefault();

          // Clear previous errors
          $errorList.empty();
          $errorHeading.empty();
          $errorContainer.hide();

          // Disable the submit button to prevent multiple clicks.
          $newSubmit.attr({disabled: true});
          $spinner.insertAfter($newSubmit);

          // Fetch details required for the createToken call to Pin Payments.
          var card = {
            number:           $('#cc-number', context).val(),
            name:             pinSettings.name,
            expiry_month:     $('#cc-expiry-month', context).val(),
            expiry_year:      $('#cc-expiry-year', context).val(),
            cvc:              $('#cc-cvc', context).val(),
            address_line1:    pinSettings.address,
            address_line2:    '',
            address_city:     pinSettings.city,
            address_state:    pinSettings.state,
            address_postcode: pinSettings.postcode,
            address_country:  pinSettings.country
          };

          // Request a token for the card from Pin Payments.
          pinApi.createCardToken(card).then(handleSuccess, handleError).done();
        });
      });

      function handleSuccess(card) {
        // Add the card token to our form.
        $('input[name="commerce_payment[payment_details][card_token]"]')
          .val(card.token);

        // Resubmit the form to the server
        // Only the card_token will be submitted to your server. The
        // browser ignores the original form inputs because they don't
        // have their 'name' attribute set.
        $newSubmit.hide();
        $spinner.remove();
        $submitButton.show();
        $submitButton.click();
      }

      function handleError(response) {
        $errorHeading.text(response.error_description);

        if (response.messages) {
          $.each(response.messages, function(index, paramError) {
            $('<li>')
              .text(paramError.message)
              .appendTo($errorList);
          });
        }

        $errorContainer.show().removeClass('element-hidden');
        $('html, body').animate({
          scrollTop: $errorContainer.offset().top
        }, 1000);

        // Re-enable the submit button.
        $spinner.remove();
        $newSubmit.removeAttr('disabled');
      }
    }
  }
})(jQuery);
