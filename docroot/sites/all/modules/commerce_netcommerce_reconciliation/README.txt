INSTALLATION
Install the module as usual from Structure -> Modules.


This module depends on commerce_netcommerce payment method.

When a payment is made using NetCommerce, NetCommerce will make a callback
request back to your site.
In some cases, this callback don't reach the website and order is payed on
NetCommerce and we don't get the notification on website.

This module will try to reconcile the transactions that didn't get back
from NetCommerce.
