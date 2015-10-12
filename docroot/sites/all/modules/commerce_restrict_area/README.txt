DESCRIPTION
-----------

Postal code, locality or administrative area filtering functionality for the 
Drupal Commerce orders. This module provides 2 main modes (whitelisting and 
blacklisting) which allow shop owners to restrict the areas of a country that 
they deliver to:

Blacklisting
If a customer tries to complete checkout with a postal code, locality or 
administrative area that is on the blacklist, they will be prevented from 
doing so. This allows shop owners to exclude a certain part of the country.

Whitelisting
If a customer tries to complete checkout with a postal code, locality or 
administrative area that is not on the whitelist, they will be prevented 
from doing so. This allows shop owners to include only a small part of the 
country (e.g. they may wish only to accept orders from their locality. 
A whitelist allows them to do so without needing to exhaustively list all 
the postal code, locality or administrative area that they do not accept).

Partial matches
Matches in both modes are done on partial postal codes, so the blacklist 
could contain "PL" which would prevent the postal code PL5 4AB from being 
submitted. Similarly you could whitelist only the "PL" postal code, which 
would prevent the postal code "BS3 4BC" from being submitted.

Billing vs Shipping
By default Drupal Commerce only has a "billing" address on the checkout. 
This module supports filtering on this address field. In addition, if 
commerce_shipping is enabled, filtering will be available on the shipping 
address.

Rules integration
You can configure rules using the actions to take if it detects an order 
with products not available for this area.


DEPENDENCIES
------------

- rules: The rules module allows site administrators to define conditionally 
    executed actions based on occurring events. [1]
- commerce [2]
  - commerce_product: Defines the Product entity and associated features.
  - commerce_order: Defines the Order entity and associated features.

[1] http://drupal.org/project/rules
[2] http://drupal.org/project/commerce


INSTALLATION
------------

1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) Visit "admin/commerce/config/restrict-area" to learn about the various 
   settings.


SPONSORS
--------

- This Drupal 7 module is sponsored by OFIPRECIOS ~ http://www.ofiprecios.com


AUTHOR
------

Manuel Garc’a (facine) ~ http://facine.es
