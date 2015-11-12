Commerce Coupon by product reference
************************************

This simple module helps you set up percentage coupons that only apply to
specific products. It uses a multi-value entity reference field attached to the
coupon to select which products will be discounted. If the field is left empty
then the coupon will be valid for all products.

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
   cart (rules_commerce_couponprodref_validate_refererenced_products).

Installation & Use
******************

1. Enable the module as normal[1].
2. It's necessary to modify the percentage coupon pricing rule. Go to 
   'Configuration -> Workflow -> Rules' and click 'edit' for the rule 'Apply
   percentage coupons to product line item'[2]. Delete the looped action called
   'Apply a percentage coupon to a product line item' and add the new looped
   action 'rule: Apply coupon to line item (checking product reference)'. Select
   'commerce-line-item' for the line item and 'list-coupon' for the coupon. (If
   you can't find 'list-coupon' then it's possible you didn't try to add an
   action _within_ the loop - make sure to click 'Add action' under 'Operations'
   for the loop.)
3. The reference field is added automatically to the bundle
   'commerce_coupon_pct' (it can be removed). The field can be added to other
   coupon types if desired.

You should now be able to create new percentage coupons that reference products.
If a customer tries to use a coupon that doesn't match any products in their
cart, then there will be a validation failure. If a customer does have one or
more valid products for that coupon, then all referenced products will be
discounted appropriately.

[1] http://drupal.org/documentation/install/modules-themes/modules-7
[2] admin/config/workflow/rules/reaction/manage/commerce_coupon_pct_apply_pct_coupons_to_line_item

author: AndyF
http://drupal.org/user/220112
