CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration
 * Features
 * Requirements
 * Further information
 * Known issues

INTRODUCTION
------------

Salsa Commerce is an extension for the Salsa Entity module and connects the
Drupal integration of Salsa with the e-commerce framework Drupal Commerce.

It basically adds modules payment methods/gateways, that are usually
available for Drupal Commerce, to donation pages as well as for paid events.
It is just replacing the salsa default payment form with the usual payment
method selection of Drupal Commerce.

For more information about the module please visit the project page at
http://www.drupal.org/project/salsa_commerce

If you find have any problems, please check the issue queue or report the
problem if it isn't such yet:
http://drupal.org/project/issues/salsa_commerce

INSTALLATION
------------

After installing and configuring the Salsa API and Salsa Entity modules, install
and enable Salsa Commerce. If you have paid events configured in on your salsa
account, you will also want to enable the Salsa Commerce Event module.

See http://drupal.org/documentation/install/modules-themes/modules-7 for more
information on how to install modules in Drupal 7.

CONFIGURATION
-------------

In order to make this module work, you won't actually have to configure anything.
It's already enough to install and enable the module. Of course you need to
setup Drupal Commerce and some payment modules in order to make it work.

For salsa donation pages and events, there are redirect fields on the salsa back-
end. If you want to make those redirects also work in Drupal, you will have to
add the following lines to your settings.php:

$conf['salsa_commerce_donate_page_redirect'] = TRUE;
$conf['salsa_commerce_event_redirect'] = TRUE;

FEATURES
--------

 * For every order, that is created by the salsa_commerce module in Drupal, a
   donation gets created in salsa.

 * The following tokens are available for simple email triggers (stream mails
   aren't working yet!)

    Paid Events
      - amount
      - Currency_Code
      - Transaction_Date
      - Event_Name
      - Fee_Reference_Name

    Donate pages:
      - Reference_Name
      - amount
      - Currency_Code
      - Transaction_Date

 * Event guests can be signed up as long as the event state is not waiting list.
   The created donation is then a sum of all event fees of the supporter and his
   guests.

 * If Commerce is configured to use the addressfield module to store the billing
   information, the supporter data is stored in there.

REQUIREMENTS
------------

* Drupal 7. There will be no D6 backport.
* Entity API
* Salsa API
* Commerce
* Salsa Entity
* A Salsa campaign manager login

FURTHER INFORMATION
-------------------

 * Project Page:
   http://drupal.org/project/salsa_commerce
 * Issue Queue:
   http://drupal.org/project/issues/salsa_commerce

KNOWN ISSUES
------------

 * At the moment, each request on the page will trigger the module to create a
   new order object. So you will end up with at least 2 orders created when
   loading the page and submitting the donation form.

 * The token replacements that are mentioned in the "features" section do not
   yet work for salsa stream mails (HTML mails). This issue can't be solved in
   Drupal and will have to be fixed by salsalabs.
