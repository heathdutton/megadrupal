INTRODUCTION
------------
transferuj.pl integration with Payment API. After installing you will be able
to add new transferuj.pl payment method. You can use it with any module that
is using Payment API for its payments, for example:

  - Ubercart with Payment for Ubercart
  - Drupal Commerce with Payment for Drupal Commerce
  - Webform with Payment for Webform

And more. See Payment module for more information.

Important:
The module doesn't support (yet) at all "chargeback" part of transferuj.pl API.


REQUIREMENTS
------------
This module requires the following modules:
* Payment (https://drupal.org/project/payment)


INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
* To add transferuj.pl payment method go to:
  Configuration » Web services » Payment » Add payment method
  and choose transferuj.pl, after filling configuration form and saving,
  the new payment method will be available.
* You can edit transferuj.pl settings in:
  Configuration » Web services » Payment » Payment methods


MAINTAINERS
-----------
Current maintainers:
* Łukasz Zaroda (Luke_Nuke) - https://drupal.org/user/1890152