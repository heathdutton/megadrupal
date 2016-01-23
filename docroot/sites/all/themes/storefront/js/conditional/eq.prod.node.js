(function ($) {
  Drupal.behaviors.ProductNode = {
    attach: function(context) {
      $("article.node-product-display .prod-column").equalHeight();
    }
  }
})(jQuery);
