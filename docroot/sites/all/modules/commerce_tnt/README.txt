INTRODUCTION
============

Commerce TNT provides integration with the TNT Shipping API to retrieve a
a shipping price depending on delivery location, total weight, number of
packages, length, width and height. 


INSTALLATION
============

 1. Install and configure the dependencies. Check their README files for
    details.
 2. Install and enable the Commerce TNT module.


DEPENDENCIES
============

Commerce TNT depends on the following modules:

 *  commerce
 *  commerce_shipping
 *  commerce_physical

 You are also required to have an account with TNT (http://www.tnt.com)


INSTALLATION & CONFIGURATION
============================

 1. Install and enable the module and all dependencies. Please make sure
    you are using the latest version of all modules.
 2. Configure your TNT RTT API settings at 
    admin/commerce/config/shipping/methods/tnt/edit
 3. Add weight, length, width and height to the product entities

Once all setup you should now get real-time shipping estimates in your
Drupal Commerce checkout process. If not then raise an issue.


MAINTAINERS
===========

 *  jbloomfield <http://drupal.org/user/834002>
