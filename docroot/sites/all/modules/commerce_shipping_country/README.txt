CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Installation
* Configuration


INTRODUCTION
------------

This module provides a simple UI allowing to limit the available countries for
shipping.
An admin can configure what is the commerce customer profile type for shipping,
and what is the addressfield that should have the countries limited.

This setting is basically the same as going to field settings for the address
field but does not require permissions to administer profile types.


REQUIREMENTS
------------
This module requires the following modules:

* Commerce Shipping (https://drupal.org/project/commerce_shipping)
* Commerce Customer, part of Commerce (https://drupal.org/project/commerce)


INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

* Configure user permissions in Administration » People » Permissions:

   - Administer commerce shipping country

     Grant permission to configure the module.

* Limit the available shipping countries in Administration » Store » 
  Configuration » Commerce shipping country
