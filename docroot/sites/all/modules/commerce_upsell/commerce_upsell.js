(function ($, Drupal, window, document, undefined) {

Drupal.behaviors.commerceUpsellPrices = {
  attach: function(context, settings) {
    if (typeof settings.commerce_upsell !== 'undefined' && typeof settings.commerce_upsell.prices !== 'undefined') {
      for (var f in settings.commerce_upsell.prices) {
        var prices = settings.commerce_upsell.prices[f],
          $widget = $('select[name=\'upsells[' + f + '][upsell_product]\']'),
          $price = $('.upsell-' + f + '-price')
          ;

          $widget.once('commerce-upsell-prices').on('change input', function() {
            var val = $(this).val();
            $price.text(prices[val]);
          });
      }
    }
  }
}

})(jQuery, Drupal, this, this.document);
