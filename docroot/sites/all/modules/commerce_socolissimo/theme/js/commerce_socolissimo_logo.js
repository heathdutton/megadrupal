/**
 * @file
 * Display So Colissimo category on shipping pane.
 */

(function ($) {

  Drupal.behaviors.commerceSocolissimoFlexibilityLogo = {
    attach: function (context, settings) {
      var firstSocolissimoService = $('.commerce_shipping .form-item-commerce-shipping-shipping-service input:radio[id*="edit-commerce-shipping-shipping-service-socolissimo"]').first();
      firstSocolissimoService.once(function () {
        $(this).parent().before('<div class="commerce-socolissimo-services clearfix">' + settings.commerce_socolissimo_logo + '</div>');
      })
    }
  }

})(jQuery);
