Description
-----------
Module implements Bulgarian EPay payment method (http://epay.bg) for Commerce.

Requirements
------------
Drupal 7.x
Commerce
Commerce Payment

Installation
------------
1. Copy the module directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Get official merchant account in epay.bg
  * for test purpose get free account in their development environment on
  https://devep2.datamax.bg/ep2/epay2_demo/

!!! IMPORTANT !!!
4. Set URL за известяване in your epay.bg profile to
http://example.com/payments/epaybg/response
(replace example.com with your domain.)

5. Add appropriate settings below "PAYMENT SETTINGS" in
Administration » Store » Configuration » Payment methods » ->
Editing reaction rule "EPayBG" and "EPayBG World".

6. "Invoice prefix" need to be unique for each instalation on use in the same epaybg profile.


Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/commerce_epaybg

Author
------
Svetoslav Stoyanov
http://drupal.org/user/717122 
