
--------------------------------------------------------------------------------
Commerce Delivery
--------------------------------------------------------------------------------

Maintainer:  xlyz, xlyz@tiscali.it

Provides basic functionalities and admin pages to handle deliveries in Drupal 
Commerce.

Poject homepage: http://drupal.org/project/commerce_delivery

Issues: http://drupal.org/project/issues/commerce_delivery

Installation
------------

 * This module depends on Drupal Commerce http://drupal.org/project/commerce

 * Copy the whole delivery_commerce directory to your modules directory
   (e.g. DRUPAL_ROOT/sites/all/modules) and activate it in the modules page

 * Products must have a boolean field to identify shippable items.

 * The administrative user interface can be found at 
   admin/commerce/config/delivery
   While some options have sensible defaults, you need to complete the 
   configuration or this module won't work properly
   

Documentation
-------------

This modules create a delivery entity. Each delivery need to be associated to
an order. Each order can have zero, one or multiple deliveries.

A delivery is automatically created on order save if it contains at least one
shippable item. Further deliveries can be manually added from the deliveries
tab under the orders menu. On the same page each order line item can be moved 
from one delivery to the other.

Views and rules modules are used as much as possible to allow easy 
customization.
 
To identify shippable items a boolean field is needed. To avoid duplication
with other modules, it's not hard coded and can be selected in the config 
page. Don't forget after a new one is selected to update the views that
use it: commerce_delivery_by_order and commerce_delivery_view.


Wish list
-----

 * delivery note as pdf
 * integration with delivery_shipping and delivery_stock
 * delivery reference field with own formatters (instead of number_integer)
 
