DESCRIPTION
-----------

This module is an extension of Drupal Commerce that provides a display
formatter for the Commerce Price field in which you can specify the decimal
places are displayed.

If you do not know how to alter the number of decimal places Commerce provides
by default, the Commerce Currency Settings [1] module can help you.

[1] http://drupal.org/project/commerce_currency_settings


FEATURES
--------

 * Display formatter with n decimals.
 * Display formatter with n decimals and components.
 * Views handler for line-item summary with n decimals.
 * Views handler for order total with n decimals.
 * Sub-Module for add support to other formatters:
   - Commerce Price Savings Formatter [1]

[1] http://drupal.org/project/commerce_price_savings_formatter


DEPENDENCIES
------------

 * Drupal Commerce Price

[1] http://drupal.org/project/commerce


DEPENDENCIES FOR SUB-MODULE
---------------------------

 * Field formatter settings

[1] http://drupal.org/project/field_formatter_settings


INSTALLATION
------------

1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) Visit manage display of any commerce price field to learn about the 
   various settings.


SPONSORS
--------

 * This Drupal 7 module is sponsored by NovaRosa.es ~ http://novarosa.es


AUTHOR
------

Manuel García (facine) ~ http://facine.es
