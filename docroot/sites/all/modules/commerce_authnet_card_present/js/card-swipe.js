/**
 * @file
 * JavaScript related to card swiping in a payment terminal.
 */
(function($) {
  Drupal.behaviors.commerceAuthnetCardPresentSwipe = {
    attach: function(context, settings) {
      $('#edit-payment-details-card-present-track', context).focus();
    }
  }
})(jQuery);
