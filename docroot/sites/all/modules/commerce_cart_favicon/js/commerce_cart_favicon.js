/**
 * @file
 * commerce_cart_favicon module javascript settings.
 */
(function($) {
  Drupal.behaviors.commerceCartFavicon = {
    attach: function (context, settings) {
      try {
	      var favicon=new Favico(
	        settings.commerceCartFavicon.settings
	      );
	      favicon.badge(settings.commerceCartFavicon.count);
      } catch (ex) {
      }
    }
  }
})(jQuery);