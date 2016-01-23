INTRO

Light Shop is a simple ecommerce solution for Drupal.

FEATURES

* Session-stored basket.
* Adding to basket and its subsequent editing/checkout process is done
using AJAX.
* Node based shoping. As the product can be assigned to any node type.
One or more. With full support Field API features (regarding nodes).
* Orders history. For each order will be created a separate node.
* There is no dependence on other modules.
* Full support for working with anonymous users

LIMITATION

No payment methods. Only e-mail notification.

USAGE

After installing the module you can configure settings by navigating
to: admin/config/lightshop.

On the settings page you need specify:
1. First tab - General Settings.
  a) The site e-mail for notifications about new orders.
  b) Specify what node  types  are product.
  c) Optionally you may specify whether to display the graph with quantity of
  the product and its price (and which field will contain this value).

2. Second tab - Notification Settings.
  a) Check the appropriate box if notification of the created order must be
  sent  to the customer also.
  b) Check the fields whose data you want to be included in the notification.

Choose the region for display basket block.

On permissions page set which roles can use the basket and make purchases.

NOTE

The project previously was called Node Baset
(www.drupalka.ru/nodebasket-module). But due to the fact that
the project with the same name already exists on drupal.org it's renamed
as Light Shop.

----------------------------------

Credits go to Dalay
(http://www.drupalka.ru/).
