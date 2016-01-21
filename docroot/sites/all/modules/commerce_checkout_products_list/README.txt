Commerce Checkout Products List
-------------------------------
Provides a checkout pane that lists all active products, allowing the visitor to
add & remove products from the current order. 

This module is a fork of the Commerce Cart Form Checkout Pane module [1]; that
module allows editing the products currently in the user's cart, whereas this
module shows all products.


Configuration
--------------------------------------------------------------------------------
Once enabled, an item named "Product list / cart list (Form)" will be available
on the checkout configuration page (admin/commerce/config/checkout). On the
configuration page for this pane it will be possible to select the view to use,
which defaults to the "Defaults" i.e "master" display of the main "Products"
view, as is bundled with the Commerce Products module.

Note: As the default Products view includes active and inactive products, it is
recommended to create a new view specially for this list.

One option available to the site builder controls whether Rules is triggered
when displaying the product prices. When Commerce Discount is used the product
price may change depending upon certain rules, which can lead to a confusing
checkout form and ultimately confuse the visitor. Changing the "Use Rules when
displaying item prices" to "Don't use Rules" will avoid triggering Commerce
Discount for the price displays. Note: this does not prevent rules and discounts
from applying to the items in the actual cart, it only affects the price shown
on the form.


Related modules
--------------------------------------------------------------------------------
Some other modules work well with this one:

* Commerce Auto-Checkout
  https://www.drupal.org/project/commerce_auto_checkout
  Make visitors skip the cart page, taking them directly to the checkout page.
  Also allows a product to be automatically added to the cart if it is otherwise
  empty.


Credits / Contact
--------------------------------------------------------------------------------
Currently maintained by Damien McKenna [2], based upon code originally written
by David Kitchen [3].

The best way to contact the author is to submit an issue, be it a support
request, a feature request or a bug report, in the project's issue queue:
  https://www.drupal.org/project/issues/commerce_checkout_products_list


References
--------------------------------------------------------------------------------
1: https://www.drupal.org/project/commerce_cart_form_checkout_pane
2: https://www.drupal.org/u/damienmckenna
3: https://www.drupal.org/u/dwkitchen
