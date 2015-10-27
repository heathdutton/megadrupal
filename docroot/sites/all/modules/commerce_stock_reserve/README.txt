Commerce Stock Reserve
======================
This is a small module that extends Commerce Stock
(https://drupal.org/project/commerce_stock) to allow "reserving" a product's
stock when a customer adds it to their shopping cart.

This removes the risk of overselling when multiple customers are attempting to
buy the same product at the same time.

Releasing reserved stock on cron
--------------------------------
There is a risk of underselling, if stock remains "reserved" in customers' carts
when sales are closed. So this module lets you release old reserved stock during
cron runs. The default is that reserved stock will be released from orders
(carts) that have not been modified for more than half an hour.

You can configure this at: admin/commerce/config/stock/reserve

This module is probably best used in conjunction with Commerce Cart Expiration
(https://drupal.org/project/commerce_cart_expiration), which deletes "abandoned"
shopping carts after a configured interval. That there are two cron jobs, one
for each module, is so that you have different time intervals: for example, you
could configure that line items with reserved stock are deleted after half an
hour, while entire carts expire after 48 hours.
