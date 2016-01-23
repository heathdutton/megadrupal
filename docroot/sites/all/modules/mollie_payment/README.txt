CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------
This module provides Mollie (https://www.mollie.nl/) integration for the
Payment platform (https://drupal.org/project/payment). Mollie is a Dutch payment
service provider. With this module enabled site owner can enable customers
(more general visitors) to perform payments to the site owners Mollie account.
Depending on this Mollie account customers can pay through e.g. iDEAL,
Mister Cash, Creditcard, PayPal (see complete list:
https://www.mollie.nl/betaaldiensten/).
 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/finlet/2006918
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2006918

REQUIREMENTS
------------
This module requires the following modules:
 * Payment (https://drupal.org/project/payment)
 * Libraries (https://drupal.org/project/libraries)
This module requires the following 3rd party libraries:
 * Mollie API client for PHP (https://github.com/mollie/mollie-api-php)
You should have an approved Mollie account (https://www.mollie.nl/) to be able
to use this module.

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * Create sites/all/libraries (if it does not already exist)
 * Put the Mollie API client for PHP (https://github.com/mollie/mollie-api-php)
   in the libraries folder so that Autoloader.php is at the path
   sites/all/libraries/mollie_api/src/Mollie/API/Autoloader.php
   ('git clone https://github.com/mollie/mollie-api-php.git mollie_api' from
   within the sites/all/libraries folder)

CONFIGURATION
-------------
After installation a payment method for Mollie should be created in the
configuration of the Payment platform.
 * Go to admin/config/services/payment
 * Click on Payment methods
 * Click on Add payment method
 * Click on Mollie in the list of available payment method types
 * Fill in a title and the Mollie API key (which can be found in the dashboard
   of your Mollie account)
 * Click the Save button
 * Important remark: Don't set a webhook URL for the website profile in your Mollie dashboard. Mollie Payment uses a
   different webhook for each payment.

MAINTAINERS
-----------
Current maintainers:
 * Rico van de Vin (ricovandevin) - https://www.drupal.org/user/1243726
This project has been sponsored by:
 * Finlet
   Finlet offers consultancy and education for Drupal.
   Visit http://finlet.nl for more information.
