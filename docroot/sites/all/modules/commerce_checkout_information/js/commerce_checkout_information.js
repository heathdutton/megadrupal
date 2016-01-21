/**
 *
 */
(function($){
  Drupal.behaviors.commerceCheckoutInformation = {
    attach: function(context, settings) {
      $('.js-commerce-checkout-information:not(.js-commerce-checkout-information-processed)', context).addClass('js-commerce-checkout-information-processed').each(function(){
        // Add click based event handling (apply/remove class)
      });
    }
  };
})(jQuery);
