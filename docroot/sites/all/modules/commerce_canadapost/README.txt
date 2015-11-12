This module provides Canada Post shipping quotes for Drupal Commerce.

Requirements
------------
  * Commerce Shipping 2.x (drupal.org/project/commerce_shipping)
  * Commerce Physical Product 1.x (drupal.org/project/commerce_physical)
  * A Canada Post Developer Program API key. See [1] for instructions (follow
    steps 1 to 3).

Configuration
-------------
Once installed, browse to the settings form [2] and enter your Customer Number
and Production API Key. Shipping services (Expedited, Xpresspost, etc) can be
enabled/disabled from this form in addition to changing basic settings like
turnaround time, origin postal code, and markup/handling rates.

You will also need to add a physical weight field (from the Physical Fields
module) to your product variation types [3] and supply weight values for your
products. If no weight can be determined for an order, the module will not
return any shipping rates. The name of the weight field does not matter.

Upgrading from 7.x-1.x
----------------------
  * run update.php
  * most settings are preserved, however shipping services will have to be
    re-selected


[1] https://www.canadapost.ca/cpo/mc/business/productsservices/developers/services/gettingstarted.jsf
[2] http://YOURSITE/?q=admin/commerce/config/shipping/methods/canadapost/edit
[3] http://YOURSITE/?q=admin/commerce/config/product-variation-types
