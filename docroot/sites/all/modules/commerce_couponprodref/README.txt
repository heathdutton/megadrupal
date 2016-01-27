Commerce Coupon by product reference
************************************

Important notice: This module is only compatible with the 1.x branch of Commerce
coupon.

This module lets you specify certain products that must be present in an order
before a particular coupon can be redeemed. For percentage coupons, the percent
discount is only applied to the referenced products. If the referenced items are
removed from the order, then the coupon is also removed.

It uses a multi-value entity reference field attached to the coupon. If the
field is left empty then the coupon will be valid for all products.

The module adds

1. A new condition for evaluating whether an order contains a product referenced
   by a coupon (commerce_couponprodref_order_has_referenced_product);
2. A multi-value entity reference field linking coupons to products
   (field_commerce_couponprodref);
3. A Rules component for applying percentage coupons to line items where they
   match the reference field
   (rules_commerce_couponprodref_apply_coupon_to_line_item_component);
4. A new coupon validation rule that won't allow a coupon to be redeemed if it
   has a populated reference field that doesn't match any of the products in the
   cart (rules_commerce_couponprodref_validate_refererenced_products);
5. A new rule that revalidates coupons after a product is removed from the cart,
   removing any with references that aren't satisfied by the new cart
   (rules_commerce_couponprodref_remove_invalid_coupons).

Installation & Use
******************

Enable the module as normal[1] and it should work without configuration. Edit a
coupon to see the product reference field.

[1] http://drupal.org/documentation/install/modules-themes/modules-7

author: AndyF
http://drupal.org/user/220112
