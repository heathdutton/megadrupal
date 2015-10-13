Commerce Checkout Multilane
===========================

Drupal commerce can be adapted to virtually any use-case. At least as long as
the checkout process remains roughly the same within one site. If this is not
the case, hook_commerce_checkout_pane_info_alter() provides ways to modify
pane order and visibility. However, because that alter-hook does not provide
any context (e.g., the current shopping cart order) its usefulness is quite
limited when it comes to dynamically rearange panes based on shopping cart
contents.

Enter Commerce Checkout Multilane. This module provides means to create
additional independent checkout sequences (lanes) with different pane
visibility and order. With this module it is possible to, e.g., provide a
separate "Express" lane used for one-click checkouts or donations.

Selection of the lane is done through the checkout-lane entity property on the
order. The property should be populated using an appropriate reaction rule,
e.g., before a new order is saved or when a product is added to the cart.


Documentation
-------------

Detailed documentation is available in the Site Building Guide
(https://www.drupal.org/node/2353089), for the API documentation, refer to the
file commerce_checkout_multilane.api.php which is part of the source tree.


Limitations
-----------

Due to the architecture of Commerce Checkout, a checkout pane will share its
configuration accross each lane. E.g. it is not possible to use the Completion
Message pane with different messages in different lanes.


Related Modules
---------------

This module is specifically designed to complement Commerce NoCart.
* https://drupal.org/project/commerce_nocart
