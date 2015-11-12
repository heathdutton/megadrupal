/**
 * @file
 * Javascript to generate Stripe token in PCI-compliant way.
 */

(function ($) {
  Drupal.behaviors.stripe = {
    attach: function (context, settings) {
      if (settings.stripe.fetched == null) {
        settings.stripe.fetched = true;

        $('#ms-stripe-payment-submit-form').submit(function(event) {
          // Prevent the form from submitting with the default action.
          event.preventDefault();

          var validated = true;

          // First check the required fields are all filled in
          $("#ms-stripe-payment-submit-form input.required").each(function(index) {
            var fieldval = $(this).val();
            if (!fieldval) {
              validated = false;
            }
          });

          if (!validated) {
            alert("Please fill out all required fields.");
            return;
          }

          $('#ms_ajax_loader').show();

          // Disable the submit button to prevent repeated clicks.
          $('#ms-stripe-payment-submit-form #edit-submit').attr("disabled", "disabled");

          Stripe.setPublishableKey(settings.stripe.publicKey);
          var full_name = $('#edit-cc-first-name').val() + $('#edit-cc-last-name').val();
          Stripe.createToken({
            name: full_name,
            address_line1: $('#edit-billing-address1').val() ? $('#edit-billing-address1').val() : '',
            address_line2: $('#edit-billing-address2').val() ? $('#edit-billing-address2').val() : '',
            address_state: $('#edit-billing-state').val() ? $('#edit-billing-state').val() : '',
            address_zip: $('#edit-billing-zip').val() ? $('#edit-billing-zip').val() : '',
            address_country: $('#edit-billing-country').val() ? $('#edit-billing-country').val() : '',
            number: $('#edit-cc-number').val(),
            cvc: $('#edit-cc-cvv').val(),
            exp_month: $('#edit-cc-exp-month').val(),
            exp_year: $('#edit-cc-exp-year').val()
          }, Drupal.behaviors.stripe.stripeResponseHandler);

          // Prevent the form from submitting with the default action.
          return false;
        });
      }
      else {
        window.location.reload(true);
      }
    },

    stripeResponseHandler: function (status, response) {
      if (response.error) {
        // Show the errors on the form.
        $("div.payment-errors").html($("<div class='messages error'></div>").html(response.error.message));

        // Enable the submit button to allow resubmission.
        $('#ms-stripe-payment-submit-form #edit-submit').removeAttr("disabled");

        $('#ms_ajax_loader').hide();
      }
      else {
        var form$ = $("#ms-stripe-payment-submit-form");
        // Token contains id, last4, and card type.
        var token = response['id'];
        // Insert the token into the form so it gets submitted to the server.
        form$.append("<input type='hidden' name='stripeToken' value='" + token + "'/>");
        // And submit.
        form$.get(0).submit();
      }
    }
  }
})(jQuery);
