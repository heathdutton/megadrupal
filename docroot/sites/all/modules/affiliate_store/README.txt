AFFILIATE STORE
---------------

This module helps in affiliate marketing by setting up a web store in your
website for selling products from merchants. This is for users who want to earn
commission by selling other people's products.

Normally in affiliate marketing, you need to download product data from
merchant, extract useful bits out of the data in order to create a post for
each product to promote, and then try to keep up to date with every product
changes.

The process is tedious and repetitive and a lot of works to do just to make
products show up correctly in your website. Yet the problem gets worst if you
promote thousand of products from several merchants at once.

Affiliate Store module helps in the process by lifting away the hard work, by
integrating solution from EarnHighway affiliate store, so that you don't need
to worry about getting and updating product data anymore, and can better
utilize your time doing useful things that directly relate to sales conversion,
like SEO, links building, or copy writing.

Visit EarnHighway at http://www.earnhighway.com for more information. Sign up
is FREE.


HUB
---

The Hub is where you specify what products to show up in your affiliate store.
Your affiliate store will be kept up to date by getting data periodically from
the Hub.

Visit EarnHighway Hub at http://hub.earnhighway.com.


FEATURES
--------

- Automatically keep up to date daily.

- Product as node. You can leverage existing modules to further enhance your
  affiliate store since node is Drupal most basic building block.

- Products are searchable through the core Search module out of the box.

- Mix products from multiple merchants of your choice. Don't see the type of
  products you want from a merchant? Then get it from another merchant.

- Three hierarchical levels of product categories support.

- Flexible category mapping. Rename category or merge multiple categories
  according to your needs.

- Site URL friendly product link. All product links exist under your website
  domain so your site users won't be confused by third party links.

- Views module integration. You can create additional product views according
  to your specific selection criteria.

- Customizable layout and theming.


REQUIREMENTS
------------

Your website must be able to make outgoing HTTP request in order to get data
from the Hub. If your website cannot make outgoing request, it will be stated
in status report page at:
  Administration > Reports > Status report

Normal PHP install has all the requirements needed by this module. So, you may
skip to the next section.

This module needs to generate Hash-based Message Authentication Code (HMAC) for
authentication with the Hub. If your PHP has HMAC disabled, you must install
the Libraries API module and PHP Secure Communications Library.

This module also requires JSON decoding support. If your PHP has JSON disabled,
you must install the Libraries API module and Services_JSON PEAR library.

Or you can simply install the Affiliate Store module and visit the status
report page to check for issue.

Libraries API module:
  http://drupal.org/project/libraries
PHP Secure Communications Library:
  http://phpseclib.sourceforge.net
Services_JSON PEAR library:
  http://pear.php.net/package/Services_JSON

See section INSTALLATION on how to install external libraries.


INSTALLATION
------------

Install Affiliate Store module as usual, see http://drupal.org/node/70151 for
more information.

In short, put the module under:
  sites/all/modules/affiliate_store
The path to affiliate_store.info file should be:
  sites/all/modules/affiliate_store/affiliate_store.info
Enable the module on the Modules administration page at:
  Administration > Modules
Configure the module settings at:
  Administration > Structure > Affiliate store
Visit your storefront at:
  http://www.example.com/affiliate-store

If your need to install external libraries to support HMAC generation or JSON
decoding (see section REQUIREMENTS), then follow the instructions below.
Libraries API module is a prerequisite for using external libraries.

To install Libraries API module, download the module and put it under:
  sites/all/modules/libraries
The path to libraries.info file should be:
  sites/all/modules/libraries/libraries.info
Enable the module on the Modules administration page at:
  Administration > Modules

To install PHP Secure Communications Library, download the package and put it
under:
  sites/all/libraries/phpseclib
The path to Hash.php file should be:
  sites/all/libraries/phpseclib/Crypt/Hash.php

To install Services_JSON PEAR library, download the package and put it under:
  sites/all/libraries/Services_JSON
The path to JSON.php file should be:
  sites/all/libraries/Services_JSON/JSON.php


UPGRADE FROM DRUPAL 6
---------------------

Upgrade path is provided for conversion of module from version 6.x to 7.x,
however you must first update to the latest 6.x version.


CONFIGURATION
-------------

By default, anonymous and authenticated users are granted permission to view
affiliate store products. You can see all of affiliate store permissions at:
  Administration > People > Permissions
Only user 1 or user with administer affiliate store permission can access
affiliate store settings page.

You must configure cron properly in order for your affiliate store to update
automatically. You can find more information about configuring cron at:
  http://drupal.org/cron
If you have lots of products, then you may need to increase your cron job
frequency in order to bring in all the product updates on time. Modules such as
Elysia Cron and SuperCron can be helpful.

As a backup, you can use the manual update page at:
  Administration > Structure > Affiliate store > Manual update

There is a product categories block that you can enable at:
  Administration > Structure > Blocks

There is a storefront menu item that you can enable at:
  Administration > Structure > Menus > Navigation

If you have Views module installed, a default view of alternative storefront is
provided.

If you need to define custom category mapping, use the interface provided by
Affiliate Store module at:
  Administration > Structure > Affiliate store > Mapping
You may need to clear your site caches to see the changes immediately if you
have caching turned on.


THEME & TEMPLATE
----------------

The storefront and product content type have default layout template that you
can based upon for creating your own custom version if you don't like their
default look. The template files can be found under module's theme directory.

More information about theming at:
  http://drupal.org/documentation/theme


ADDITIONAL INFORMATION
----------------------

Product nodes will be automatically added, updated, or deleted during affiliate
store update. So you generally should not modify the product nodes as the
changes can be lost. This also means that a product can possibly disappeared
after update due to changes made on the merchant side: temporarily out of
stock, not sell anymore, product's unique ID or category changed, etc.

If you uninstall Affiliate Store module, all its product nodes and associated
taxonomy terms and vocabularies will also be deleted.


CONTACT
-------

Sam Men Wee (mwsam)
http://drupal.org/user/691740

Developed by EarnHighway - Affiliate Marketing with Drupal
Visit http://www.earnhighway.com for more information.
