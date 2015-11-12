/**
 * @file
 * Commerce iATS USB card reader JavaScript functionality.
 */

(function ($) {

  Drupal.behaviors.commerce_iats_usb_reader_credit_card = {
    attach: function (context, settings) {
      var encrypted_element = $("textarea#edit-payment-details-encrypted-credit-card-encrypted-number", context);
      var $status_element = $("#edit-payment-details-encrypted-credit-card-status", context);

      // When the card data field has focus, instuct the user to swipe the card.
      encrypted_element.focusin(function () {
        $status_element.html(Drupal.t('Swipe credit card now.'));
      });

      // When the card data field loses focus, instruct the user to regain focus.
      encrypted_element.focusout(function (e) {
        // Create a swipe link with a defined click event.
        var swipe_link = $('<a></a>').attr('href', '#').html(Drupal.t('Click to swipe credit card')).click(function(event) {
          event.preventDefault();
          encrypted_element.focus();

          // The focus event set in attach() is not caught when setting focus in JS. Have to set status text manually here.
          $status_element.html(Drupal.t('Swipe credit card now.'));
        });

        $status_element.html(swipe_link);
      });

      encrypted_element.focus();
    }
  }

})(jQuery);
