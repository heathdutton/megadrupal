Commerce NoCart
===============

This module provides means to build one-product fast checkout features without
touching a users shopping cart. Still the module heavily reuses the Commerce
Cart implementation and as a result NoCart-orders behave almost exactly the
same way as regular ones. I.e. the same rules events are fired (e.g. pricing),
the same alter hooks get invoked and even the add-to cart field formatter with
all its Ajax magic is leveraged by this module.

Setup
-----

 1. Change the add-to-cart settings on a product display:
    * Create a product display or edit an existing one.
    * Navigate to the "Manage Display" tab and choose "Add to Cart form" as a
      formatter for the product reference field name.
    * Enable the "Direct checkout" checkbox, change the Button text and the
      initial checkout page if appropriate.
    * Save the configuration.

 2. Place the checkout-panes into the "No cart" checkout pages.
    * Navigate to the "Store configuration" and from there to the "Checkout
      settings" screen.
    * Move all panes from the "Checkout", "Review" and "Shipping" pages to
      their "No cart" counterparts.
    * Save the configuration.

This setup works well for sites using no-cart checkout exclusively. However,
if different checkout procedures need to coexist on one site, the Commerce
Checkout Multilane module should also be installed.

Related Modules
---------------

This module works well together with Commerce Checkout Multilane.
* https://drupal.org/project/commerce_checkout_multilane

Commerce Express Checkout provides similar features.
* https://drupal.org/project/commerce_express_checkout
