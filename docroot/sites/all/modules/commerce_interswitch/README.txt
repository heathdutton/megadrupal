INTRODUCTION
------------
The Commerce Interswitch module provides the Interswitch WebPay payment method
for Drupal Commerce. The design is of this module has been successfully checked
for all the mandatory requirements of Interswitch's User Acceptance Test as
described in Interswitch's DIY document.

Features
* View of transaction history of all Interswitch WebPay payments at
admin/commerce/orders/interswitch-webpay-payments with the option to manually
check the status of the transaction.
* The Interswitch charge of 1.5% can be added to the order total.
* Rules configuration options for the Interswitch charge.


REQUIREMENTS
------------
This module requires the following modules:
  * Drupal Commerce (https://www.drupal.org/project/commerce)
  * Chaos tool suite (https://www.drupal.org/project/ctools)
  
INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
  * Configure the module from Drupal Commerce's payment method configuration
  interface at admin/commerce/config/payment-methods

  * Further configuration of the Interswitch Charge can be made at 
  admin/config/workflow/rules/reaction/manage/set_interswitch_charge

