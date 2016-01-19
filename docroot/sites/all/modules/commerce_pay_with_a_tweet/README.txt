Commerce Pay with a tweet
=========================

Description
-----------

Commerce Pay with a tweet extends pay_with_a_tweet module.
(https://drupal.org/project/pay_with_a_tweet).

Pay_with_a_tweet description:
"Pay with a tweet implements tweet payment feature. This allows the website to
make downloads available to users with a tweet as payment."

- Commerce Pay with a tweet implements commerce payment module to provide
pay with tweet payment on checkout process (payment option list).

- Create specific tweet with tokens
Available tokens: see commerce_product_tokens()
and "site" (https://drupal.org/node/390482#drupal7tokenslist)

- Commerce Pay with a tweet display the best tweet thanks to "tweet reference"
field on product and product display.
(weight/priority: Product > Product display > default tweet)


Dependencies
------------

- Pay with a tweet (= 7.x-1.0)
https://drupal.org/project/pay_with_a_tweet

- Drupal Commerce and all of its dependencies
https://drupal.org/project/commerce
Enable commerce_payment module

- Entity Reference
https://drupal.org/project/entityreference


Installation
------------
- Please check that you have a commerce product type
(admin/commerce/products/types) and
  a product display
(node type with product reference field - admin/structure/types)
- Enable "Commerce pay with a tweet" module and all dependencies
from modules pages
  Home > Administration > Modules (admin/modules)


Configuration
-------------
Configure pay_with_a_tweet module before.

The main Commerce pay with a tweet admin page (not in the same place):

- Commerce kickstart 2:
Home > Administration > Store settings > Advanced store settings > Commerce PWAT
(admin/commerce/config/commerce_pay_with_a_tweet_config)

- Drupal commerce framework:
Home > Administration > Store > Configuration > Commerce pay with a tweet
(admin/commerce/config/commerce_pay_with_a_tweet_config)


Credits
-------

The original module is written by Florian LE BRENN - http://florianlebrenn.fr
(florian.lebrenn [at] gmail [dot] com).

This module extends pay_with_a_tweet module
 written by Rolando CALDAS SANCHEZ - http://rolandocaldas.com

Current maintainers:
  Florian LE BRENN - http://florianlebrenn.fr
