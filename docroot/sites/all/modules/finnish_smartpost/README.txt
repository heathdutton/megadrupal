INTRODUCTION 
============ 
Commerce Finnish Smartpost integrates the Finnish
Smartpost / Pakettiautomaatti (http://posti.fi/yritysasiakkaat/posti-palvelee
/sahkoinen-asiointi/smartpost.html) to Drupal. It allows customers to choose a
pickup point from the nearest Smartposts.

REQUIREMENTS
============
This module requires the following modules:
* Drupal Commerce (https://www.drupal.org/project/commerce)
* Commerce Shipping (https://www.drupal.org/project/commerce_shipping)

INSTALLATION 
============ 
Install as you would normally install a contributed
Drupal module. See: https://drupal.org/documentation/install/modules-
themes/modules-7 for further information.

CONFIGURATION
=============
Configure the module at admin/commerce/config/advanced-
settings/commerce_finnish_smartpost :
1. Enable the module from Modules list.
2. Configure the module.
  2.1. Select the number of locations to show.
  2.2. Select the shipping cost.
  2.3. Select the currency for shipping cost.
  2.4. Remember to clear caches after changes.
3. The module is up and running, and allows customers to choose their pickup
point.

The information about the chosen Smartpost location is stored to the field
commerce_smartpost_address of the order. The module requires the usage of
Shipping information, and will not work if that is disabled. The module also
requires the customer to give their phone number, since the Smartpost system
requires it.

MAINTAINERS
============
Current maintainer:
 * Teemu Aro (teemuaro) - https://www.drupal.org/user/3014169

This project has been sponsored by:
 * Drupaletti Oy (http://www.drupaletti.fi)
