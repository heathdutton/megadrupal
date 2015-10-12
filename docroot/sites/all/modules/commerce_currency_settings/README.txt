DESCRIPTION
-----------

This module help you to alter Commerce currency definitions.

By default Commerce provides all active currencies according to ISO 4217.
This module allows you to change the formatting properties of existing
definitions.

Additionally, because every currency's default conversion rate is 1, this
module can be used to populate currency conversion rates with meaningful
values. Conversion rates can be calculated using any currency as the base
currency as long as the same base currency is used for every rate.


DEPENDENCIES
------------

 * Drupal Commerce (http://drupal.org/project/commerce)
 * Drupal Commerce UI (http://drupal.org/project/commerce)


INSTALLATION
------------

1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) Visit "admin/commerce/config/currency/custom-settings" to learn about
   the various settings.


SPONSORS
--------

 * This Drupal 7 module is sponsored by NovaRosa.es ~ http://novarosa.es


AUTHOR
------

Manuel García (facine) ~ http://facine.es
