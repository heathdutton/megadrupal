CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * FAQ
 * Maintainers

INTRODUCTION
------------
The commerce_moolah module allow a Drupal Commerce store to user moolah.io
services. Customer will be allowed to pay in crypto currencies such as:
Bitcoin, Litecoin, Dogecoin, Vertcoin.
 * For a full description of the module, visit the project page:
  https://drupal.org/sandbox/gaetanp/2207199

REQUIREMENTS
------------
This module requires the following modules:
* Commerce
* Commerce_ui
* Commerce_payment
* Commerce_order
* Entity

INSTALLATION
------------
 * Install as usual, see https://drupal.org/documentation/install/modules-themes/modules-7 for further information.

CONFIGURATION
-------------
 * Configure payment method in Store settings » Advanced store settings » Payment methods:
  - Edit Moolah.io payment method
    Enter API key, ipn secret. Found in https://moolah.io/merchant/api
    Enter your guids. Found in https://moolah.io/merchant (click on each currencies) 
  - Enable the payment method

MAINTAINERS
-----------
Current maintainers:
 * Gaëtan Petit (gaetanp) - https://drupal.org/project/user/2344912
