/**
 * @file
 * Hides checkout help
 */
(function ($) {
  Drupal.behaviors.commerceKlarnaCheckoutHide = {
    attach: function(context, settings) {
      $(".checkout-help").hide();
    }
  };
})(jQuery);
