README file for Commerce Affirm

CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* How it works
* Troubleshooting
* Maintainers

INTRODUCTION
------------
This project integrates Affirm payment Gateway into the Drupal Commerce
payment and checkout systems.
https://www.affirm.com/merchants/
* For a full description of the module, visit the project page:
  https://www.drupal.org/project/commerce_affirm
* To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/commerce_affirm


REQUIREMENTS
------------
This module requires the following modules:
* Submodules of Drupal Commerce package (https://drupal.org/project/commerce)
  - Commerce core
  - Commerce Payment (and its dependencies)
* PHP cURL library (http://php.net/manual/en/curl.setup.php)
* Affirm Merchant account (http://help.merchants.affirm.com)


INSTALLATION
------------
* Install as you would normally install a contributed drupal module.
  See: Installing modules (Drupal 7) documentation page for further information.
  https://drupal.org/documentation/install/modules-themes/modules-7


CONFIGURATION
-------------
* Permissions: There are no specific permissions for this module.
  The Payments permissions are to be used for configurations.
* Enable the default "Affirm Payment" method or create a new one of this type.
* Configure "Affirm Payment" payment
  Edit "Enable payment method: Affirm Payment" action (settings).
  - Transaction mode: either if a test/development store or a production one.
    Available options: "Live transactions in a live account"
    and "Test transactions in a sandbox account";
  - Private Key: Merchant private key for the payment gateway;
  - Public Key: Merchant public key for the payment gateway;
  - Financial Product Key: Merchant financial product key for the payment gateway;
  - Default credit card transaction type:
    Available options:
    Authorization and capture
    Authorization only (requires manual or automated capture after checkout);


HOW IT WORKS
------------

* General considerations:
  - Shop owner must have an Affirm Merchant account
    https://www.affirm.com/merchants
  - Customer should have an Affirm account or he will be asked to create one.
    https://www.affirm.com/u/#/signup

* Customer/Checkout workflow:
  This is an Off-Site payment method
  Redirect customer from checkout to the payment service and back.
  - Customer redirected to Affirm where an Affirm account is needed
    Sign In or Create Account;
  - After the payment confirmation on Affirm side the Customer will be
    redirected  back to the store to complete the order checkout.

* Back-end workflow:
  - "Authorization only (requires manual or automated capture after checkout)"
    transaction type:
    Authorization charge call to Affirm.
    On success a charge object is returned to be used for future operation.
    It requires later capturing of the charge returned.
  - "Authorization and capture" transaction type:
    2 calls to Affirm - one for authorization charge and second for capturing
    the charge returned.

* Store Admin workflow:
  Payment Terminal (Order admin tab)
  - Capture: for "Authorization only" payments.
  - Void: for "Authorization only" payments that were not captured.
  - Refund: for the captured payments, partial (multiple) refunds available.


TROUBLESHOOTING
---------------
* No troubleshooting pending for now.


MAINTAINERS
-----------
Maintainers:
* Julien Dubreuil (JulienD) - https://www.drupal.org/u/juliend
* Pete Barnett (petebarnett) - https://www.drupal.org/u/petebarnett
* Sam Berry (samberry112) - https://www.drupal.org/u/samberry112
* Tavi Toporjinschi (vasike) - https://www.drupal.org/u/vasike

This project has been developed by:
* Commerce Guys
  Commerce Guys are the creators of and experts in Drupal Commerce,
  the eCommerce solution that capitalizes on the virtues and power of Drupal,
  the premier open-source content management system.
  We focus our knowledge and expertise on providing online merchants with
  the powerful, responsive, innovative eCommerce solutions they need to thrive.
  Visit https://commerceguys.com/ for more information.
