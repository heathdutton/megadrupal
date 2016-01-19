CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------
This module provides Authorize.Net Card Present integration for Drupal
Commerce. Capture, credit, and void transactions are supported.
This module is designed to be used with a USB credit card reader to allow
for card swiping in the payment terminal, or by extension, through the
Commerce POS module.
 * For a full description of the module, visit the project page:
   https://drupal.org/project/commerce_authnet_card_present
 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/commerce_authnet_card_present


REQUIREMENTS
------------
This module requires the following modules:
 * Drupal Commerce (https://drupal.org/project/commerce)
 * Commerce Authorize.Net (https://drupal.org/project/commerce_authnet)


RECOMMENDED MODULES
-------------------
 * Commerce Point of Sale (POS) (https://www.drupal.org/project/commerce_pos):
   When enabled, this module provides an AJAX interface for cashiers to
   easily add products to a customer's order, as well as remove them, accept
   payment, and print receipts. The Authorize.Net Card Present payment method
   appears as 'Credit card (card present)' in this interface.


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * You will need a USB credit card reader that meets ISO 7811 standards to take
   advantage of this module's functionality.


CONFIGURATION
-------------
 * Enable the payment method rule titled Authorize.Net AIM - Card Present via
   Store > Configuration > Payment methods, and edit its action to use your
   Authorize.Net Card Present API credentials.
 * You must also choose whether you want to perform Authorization and capture
   or Authorization only transactions during checkout. Authorization only
   transactions may be captured after the fact using a capture amount that is
   less than or equal to the amount authorized.
 * The Payment checkout pane must come after the Billing information checkout
   pane for Drupal Commerce to be able to populate the order used to generate
   transactions with a billing address before this module attempts to process
   payment.


MAINTAINERS
-----------
Current maintainers:
 * Thomas Fleming (tidrif) - https://www.drupal.org/user/826568
 * Jeff Landfried (jiff) - https://www.drupal.org/user/1636348
This project has been sponsored by:
 * LAST CALL MEDIA
  A Drupal-focused web development company based in Northampton, Massachusetts.
  Visit https://lastcallmedia.com/ for more information.
