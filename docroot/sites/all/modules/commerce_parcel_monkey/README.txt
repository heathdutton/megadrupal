INTRODUCTION
============

Commerce Parcel Monkey provides integration with the Parcel Monkey
Shipping API to retrieve a shipping price depending on delivery location,
total weight, length, width and height.


API DOCS
============

The API docs can be found at http://www.parcelmonkey.co.uk/prepay.php

You will need to create an account first. It is FREE!


INSTALLATION
============

 1. Install and configure the dependencies. Check their README files for
    details.
 2. Install and enable the Commerce Parcel Monkey module.


DEPENDENCIES
============

Commerce Parcel Monkey depends on the following modules:

 *  commerce
 *  commerce_shipping
 *	commerce_physical
 *	physical


INSTALLATION & CONFIGURATION
============================

 1. Install and enable the module and all dependencies. Please make sure
    you are using the latest version of all modules.
 2. Configure your Parcel Monkey API settings at 
    admin/commerce/config/shipping/methods/parcel_monkey/edit
 3. Add total weight, length, width and height to the product entities

Once all setup you should now get real-time shipping estimates in your
Drupal Commerce checkout process. If not then raise an issue.


MAINTAINERS
===========

 *  jbloomfield <http://drupal.org/user/834002>
