/**
 * @file
 * Script file to scroll to google map after selecting shipping service.
 */

(function ($, Drupal)
{
  Drupal.behaviors.commerceBpostGoToMap = {
    attach: function (context, settings) {
      $('#edit-commerce-shipping-shipping-service').once(function () {
        // Do not scroll if an error message is displayed
        if ($('.messages.error', $('form#commerce-checkout-form-shipping')).length === 0) {
          $.fn.commerceBpostScrollToMap($(this));
        }
        $(this).change(function () {
          $.fn.commerceBpostScrollToMap($(this));
        })
      });
    }
  }

  $.fn.commerceBpostScrollToMap = function (context) {
    var shipping_service = $('input[name="commerce_shipping[shipping_service]"]:checked', context).val()
    if (shipping_service == 'bpost_bpack' || shipping_service == 'bpost_postoffice_postpoint') {
      $('html, body').animate({
        scrollTop: $('#commerce-shipping-service-details').offset().top - 50
      }, 2000);
    }
  }

}(jQuery, Drupal));
