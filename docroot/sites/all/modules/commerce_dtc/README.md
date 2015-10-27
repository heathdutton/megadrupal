Introduction
============

Have you ever wanted to provide a link to a customer that drops one or more
products directly into the cart? Here's your magic bean!

For example: http://example.com/product/magic-beans/dtc?color=green&size=small will
select the small green magic bean variant and add it directly to the cart. And
it works with bundles!!

Note: Subpathauto module (or similar) required to work on friendly URLs. Without
this module the url would be http://example.com/node/3/dtc?color=green&size=small

Each attribute (term reference) field can be mapped to the name used in the
querystring. This helps avoid poorly named fields and cleans up the querystring.
So field_bean_colors can be mapped to 'color' in the querystring.


Main Features
-------------

* Map each attribute (term reference) field to a querystring name.

* Add product(s) to the cart based on attribute values in the querystring.

* Works with standard product displays and bundles!!

* Magic quantity (qty=#) querystring parameter allowed.


Configuration
=============

Navigate to admin/commerce/config/commerce_dtc where a list of term reference
fields on product entities will be listed. Provide the querystring name to use
for each field.

Note: If commerce_pdas is enabled there will be an option to pull from that
configuration since they are essentially the same.


Recommendations
===============

Commerce PDAS (commerce_pdas) provides attribute selection on product displays
based on the querystring.

Sub-pathauto (subpathauto) allows for adding the /dtc at the end of an alias
instead of node/#/dtc.


Maintainers
===========

* Craig Aschbrenner <https://www.drupal.org/user/246322>
