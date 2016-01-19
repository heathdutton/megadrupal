Introduction
============

Have you ever wanted to link a customer to a product page with specific
attributes selected? This module provides that capability. Simply include the
attributes in the querystring on the product display page and the attributes
will be selected.

For example: http://example.com/product/magic-beans?color=green&size=small will
select the color and size on the add-to-cart form of that product display.

Each attribute (term reference) field can be mapped to the name used in the
querystring. This helps avoid poorly named fields and cleans up the querystring. So field_bean_colors can be mapped to 'color' in the querystring.

Doesn't Commerce Product URLs (commerce_product_urls) already provide this
functionality? Why yes, yes it does!

Both modules attempt to solve the same problem but with two different
approaches. Commerce Product URLs uses a product reference hook which works
great on single product displays, however when it comes to Commerce
Bundles (commerce_bundle), it failed miserably. The product reference hook
doesn't seem to play well with bundles.

So this modules was created in an attempt to solve the problem for both single
product displays and product bundles. Javascript is used to read the
querystring, locate and update the attribute fields on the product displays.


Main Features
-------------

* Map each attribute (term reference) field to a querystring name.

* Select the value of the attribute field based on the value in the querystring.

* Works with standard product displays and bundles!!


Configuration
=============

Navigate to admin/commerce/config/commerce_pdas where a list of term reference
fields on product entities will be listed. Provide the querystring name to use for each field.


Recommendations
===============

Definitely check out Commerce Product URLs. It works great on standard product
displays.


Maintainers
===========

* Craig Aschbrenner <https://www.drupal.org/user/246322>
