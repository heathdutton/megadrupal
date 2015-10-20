
Ubercart E-balance Payment Method MODULE FOR DRUPAL 7.x
-------------------------------------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Requirements
   * Installation
   * Configuration
   * Maintainers

DESCRIPTION:
------------

This module integrates with the Ubercart module and creates a new kind
of payment method where user do not requires to checkout via any other
payment gateway. User can directly purchase products through this
new payment method without putting their card details. This payment method
works on balance points provided by balance_tracker module.

Once admin credit points regarding users, then that user can purchase
products basis on those credit points and can checkout through system
without go to any other payment gateway.


REQUIREMENTS:
------------

Requires the following modules:

  - Ubercart (https://www.drupal.org/project/ubercart)
  - Balance Tracker (https://www.drupal.org/project/balance_tracker)


INSTALLATION:
-------------
Install as you would normally install a contributed Drupal module.
See:
https://www.drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION:
--------------

For configuration option you need to follow payment method page
(admin/store/settings/payment).
You can see "E-balance Payment Method" in the list,
here You can add condition for this payment method.


MAINTAINERS
--------------------------------------------------------------------
 Current maintainer:
 * Vikas kumar - https://www.drupal.org/u/babusaheb.vikas
