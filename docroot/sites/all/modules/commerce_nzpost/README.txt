
-- SUMMARY --

Integrates NZPost postage estimation with Commerce Shipping.

For a full description of the module, visit the project page:
  https://drupal.org/sandbox/DNZ/2052157

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/2052157


-- REQUIREMENTS --

commerce
http://drupal.org/project/commerce

commerce_shipping (7.x-2.x)
http://drupal.org/project/commerce_shipping

commerce_physical
http://drupal.org/project/commerce_physical

This module requires an API key with NZPost to access the shipping calculations. 
You can apply for one free here: developer@nzpost.co.nz

-- INSTALLATION --

* Install as usual:
see http://drupal.org/documentation/install/modules-themes/modules-7 


-- CONFIGURATION --

* Go to Store -> Shipping -> Shipping Methods
* Edit the NZPost Method
  * Put in your NZPost API key
  * Select the shipping methods you wish to make available in your store
* Go to Store -> Products -> Product Types -> Edit your product type
  * Add physical dimensions and physical weight to your product
* Update your available products with physical details
* Done! (See your shipping checkout page).

-- CONTACT --

Current maintainers:
* Andy Dopleach - http://drupal.org/user/2261214
