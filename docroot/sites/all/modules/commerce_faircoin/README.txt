INTRODUCTION
------------
This module provides faircoin integration with drupal commerce.
It creates a new faircoin currency and a payment method.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/xavip/2551829

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2551829

REQUIREMENTS
------------
This module requires the following modules:

 * Commerce (https://www.drupal.org/project/commerce)
 * Libraries API 2 (https://drupal.org/project/libraries)

This module requires the following js libraries:

 * jquery.qrcode
   Download: https://github.com/XaviP/jquery-qrcode/archive/master.zip
   Author jeromeetienne: https://github.com/jeromeetienne

INSTALLATION
------------
 * Download and install Commerce and Libraries API 2 modules.

 * Download the qrcode.js plugin from
   https://github.com/XaviP/jquery-qrcode/archive/master.zip
   Unpack and rename the plugin directory to "qrcode" and place it
   inside the "sites/all/libraries" directory. Make sure the path
   to the plugin file becomes:
   "sites/all/libraries/qrcode/jquery.qrcode.min.js"

 * Enable Commerce FairCoin module.

CONFIGURATION
-------------
 * Once the module is enabled, the new currency is available in
   Store > Configuration > Currency settings
   (admin/commerce/config/currency)
   Select 'FAC FairCoin FAIR' as the default store currency.

 * Edit reaction rule "FairCoin payment" in
   Store > Configuration > PaymentMethods
   (admin/commerce/config/payment-methods)
   There you can fill up the Payment Settings:
     - FairCoin address: the address where your costumers will send
       you the payments.
     - Payment instructions: instructions for customers on the final
       checkout page.

 * Enable FairCoin payment rule.

 * Give permissions to the role of your costumers (access checkout,
   view own orders of any type), create products and product displays
   (be sure the prices of your products are in FAIR currency) and make
   shopping cart block visible.

FAIRCOIN INFORMATION
--------------------
Faircoin promotes equality and a fair economy. It's endorsed by FairCoop,
the Earth cooperative for a fair economy. Please, use this software to trade
with products or services that respect these principles.

For more information, visit:
https://fair-coin.org
https://fair.coop

MAINTAINERS
-----------
Current maintainers:
 * Xavi Paniello (XaviP) - https://www.drupal.org/u/xavip
