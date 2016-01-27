Description
-----------
Module implements Bulgarian EasyPay payment method for Commerce.

On "Demo site" settings for test purpose have form to make order (payment code)
PAID - just submit received code - view form at "easypay/simulate-payment".

Requirements
------------
Drupal 7.x
Commerce
Commerce Payment
PHP (core module)

Installation
------------
1. Copy the module directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Get official merchant account in epay.bg
  * for test purpose get free account in their development environment on
  https://devep2.datamax.bg/ep2/epay2_demo/
  
  !!! IMPORTANT !!!
  If you use commerce_epaybg module too
  Set URL за известяване in your epay.bg profile to
  http://example.com/payments/epaybg/response
  (replace example.com with your domain.)
  else
  Set URL за известяване in your epay.bg profile to
  http://example.com/payments/easypay/response
  (replace example.com with your domain.)

4. Add appropriate settings below "PAYMENT SETTINGS" in
Administration » Store » Configuration » Payment methods » ->
Editing reaction rule "EasyPay"

Versions
--------
7.x-1.x

ToDo
----

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/commerce_easypaybg

Author
------
Svetoslav Stoyanov
http://drupal.org/user/717122