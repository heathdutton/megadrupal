/**
 * @file
 * Custom JS for the Commerce Checkout Products List module.
 */

(function($) {
  'use strict';

  Drupal.behaviors.ccpl_quantities = {
    attach: function(context, settings) {
      // Get the product ID for the current button.
      var get_product_id_from_link = function(this_obj) {
        // Most browsers should support this by now.
        if (typeof this_obj.dataset !== 'undefined' && typeof this_obj.dataset.product_id !== 'undefined') {
          return this_obj.dataset.product_id;
        }

        // Older versions of IE don't support the dataset attribute.
        else {
          var attributes = this_obj.attributes;
          for (var i = 0; i < attributes.length; i++) {
            if (attributes[i].name === 'data-product_id') {
              return attributes[i].value;
            }
          }
        }

        // This shouldn't happen, but you never know.
        return 0;
      }

      // Decrease the quantity every time the '-' button is clicked.
      $('#edit-cart-contents-form .decrement').click(function() {
        // Load the product ID.
        var product_id = get_product_id_from_link(this);
        // The text field corresponding to this link button.
        var $field = $('input[name="product_' + product_id + '"]');
        // The current quantity for this product.
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
        // Load the product ID.
        var product_id = get_product_id_from_link(this);

        // The text field corresponding to this link button.
        var $field = $('input[name="product_' + product_id + '"]');

        // The current quantity for this product.
        var quantity = parseInt($field.val());

        // Increase the quantity.
        $field.val(quantity + 1);
      });
    }
  }
})(jQuery);
