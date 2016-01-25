/**
 * Displaying the FuturePay Content
 * @namespace commerce_futurepay
 */

/**
 * Display the FuturePayForm.
 */
function FuturePayIntialiseForm() {
  var count = 3000;
  var $container = jQuery('#futurepay-container').parent();
  // Add the ajax throbber.
  $container.children('#futurepay-container').append('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><div class="message">' + Drupal.t('Loading library...') + '</div></div>');
  // Run it every 0,1 second.
  var counter = setInterval(function () {
    count = count - 1;

    if (typeof(FP) != 'undefined' && typeof(FP.CartIntegration) != 'undefined') {
      clearInterval(counter);
      //  Load the FuturePay content
      FP.CartIntegration();

      // Show the signup form.
      FP.CartIntegration.displayFuturePay();

      // Go to page top.
      jQuery('body').animate({scrollTop: 0}, 500);

      $container.html(FP.CartIntegration.getFormContent()).append('<input class="form-submit" type="button" id="futurepay-checkservices" value="Submit">');

      // Hide the submit button when admin.
      jQuery('#edit-submit').addClass('element-invisible');

      // Bind form submit button to the FuturePay own submit.
      jQuery('#futurepay-checkservices').bind('click', function () {
        jQuery(this).attr("disabled", "disabled");
        FP.CartIntegration();
        FP.CartIntegration.placeOrder();
        jQuery(this).removeAttr("disabled");

      });
    }

    if (count <= 0) {
      clearInterval(counter);
    }
  }, 100);
}

/**
 * Handle the validation payment response from FuturePay services.
 *
 * @param response
 *
 */
function FuturePayResponseHandler(response) {
  if (response.error) {
    //  Display some message informing your customer that
    //  the purchase was not completed
    alert(response.message);
  }
  else {
    // We are ensuring a positive response code has been forwarded by FuturePay
    // services.
    if (response.code && (response.code == 'CREATED' || response.code == 'SIGNUP_OK_AND_PURCHASED')) {
      //  Pass the returned TransactionID to Drupal.
      jQuery.ajax({
        type: "GET",
        dataType: "json",
        url: Drupal.settings.basePath + "commerce-futurepay/validate-transaction/" + response.transaction_id,
        success: function (msg) {
          if (typeof(msg.OrderStatusCode) != 'undefined') {
            if (msg.OrderStatusCode == 'ACCEPTED') {
              // Submit the checkout form.
              jQuery("#edit-continue,#edit-submit").click();
            }
            else {
              alert(Drupal.t('The transaction has been declined, please choose an another payment method.'));
            }
          }
        },
        error: function () {
          alert(Drupal.t('Something goes wrong, please contact the website administrator.'));
        }
      })
    }
  }
}
