/**
 * @file
 * Custom JS for the Commerce Checkout Products List module.
 */

(function($) {
  'use strict';

  Drupal.behaviors.ccpl_quantities = {
    attach: function(context, settings) {
      // Decrease the quantity every time the '-' button is clicked.
      $('#edit-cart-contents-form .decrement').click(function() {
        var product_id = this.dataset.product_id;
        var $field = $('input[name="product_' + product_id + '"]');
        var quantity = parseInt($field.val());
        // Cannot drop the quantity lower that zero.
        if (quantity == 0) {
          // Doh!
        }
        // Reduce the quantity.
        else {
          quantity = quantity - 1;
          $field.val(quantity);
        }
      });

      // Increase the quantity every time the '+' button is clicked.
      $('#edit-cart-contents-form .increment').click(function() {
        var product_id = this.dataset.product_id;
        var $field = $('input[name="product_' + product_id + '"]');
        var quantity = parseInt($field.val());
        // Increase the quantity.
        $field.val(quantity + 1);
      });
    }
  }
})(jQuery);
