DESCRIPTION
-----------

Add fixed fees to your orders in Drupal Commerce.

You can configure rules to trigger a fee addition to a given order, 
examples of this are adding a shipping fee if the delivery 
is overseas or charge extra for cash on delivery payment method.

Commerce fees will add a new line item to the order with 
the arbitrary amount configured.


INSTALLATION
------------

1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) Visit "admin/commerce/config/currency/custom-settings" to learn about
   the various settings.


DOCUMENTATION
-------------

1) Go to admin/commerce/config/fees and add a new fee type.

2) Add a rule at admin/config/workflow/rules

   - Event: "Apply a fee to a given order".
   - Set the conditions.
   - Action: "Apply a fee to an order".
     - Entity data selector: "commerce-order".
     - Fee: Your fee type.
     - Set amounts, currency and taxes as necessary.

Notice: if using fees based on payment type, be sure to move the payment type
pane to a screen before the review pane on admin/commerce/config/checkout so
that customers can see the fee before accepting.


SPONSORS
--------

 * This Drupal 7 module is sponsored by NovaRosa.es ~ http://novarosa.es


AUTHOR
------

Manuel García (facine) ~ http://facine.es
