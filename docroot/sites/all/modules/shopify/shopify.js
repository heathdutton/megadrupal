/**
 * @file
 * Contains javascript specific to Shopify module functionality.
 */
(function ($) {

  Drupal.shopify = {
    ctx: {},
    settings: {}
  };

  /**
   * Display an "Added to cart" message by sending a POST request to the backend.
   */
  Drupal.shopify.display_add_to_cart_message = function () {
    var $forms = $('form.shopify-add-to-cart-form');
    $forms.unbind('submit').submit(function (e) {
      var $form = $(this);
      e.preventDefault();
      $.post(Drupal.settings.basePath + '?q=shopify/added-to-cart', {
        product_id: $form.data('product-id'),
        variant_id: $form.data('variant-id'),
        quantity: $form.find('input[name="quantity"]').val()
      }, function (data) {
        $form.get(0).submit();
      });
    });
  };

  Drupal.behaviors.shopify = {
    attach: function (context, settings) {
      Drupal.shopify.ctx = context;
      Drupal.shopify.settings = settings;
      Drupal.shopify.display_add_to_cart_message();
    }
  }

}(jQuery));