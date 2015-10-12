Integral Community Exchange System:

Integral Community Exchange System or IntegralCES is a set of modules to deploy
a community exchange network using social currency (also known as complementary
currency or local currency). 

This project wants to be a new version of the popular software Community 
Exchange System (http://www.ces.org.za) but it is not officially supported by
its authors.

=======

Installation:

1) Install and enable this module.
2) Setup the initial page to /ces/blog
3) Setup blocks.

=======

Module structure:

There is a main virtual module called ices. This module does not have any
feature. It is used as a container for the other modules and contains several
general purpose files.

From ices there hang several modules:

 - ces_bank: The main module with the accounting feature. Defines exchange 
   groups, accounts, transactions...
 - ces_blog: A simple blog that is different for each exchange.
 - ces_develop: Developer utilities. Do not enable in a production site.
 - ces_import4ces: Features import data from CES .csv files.
 - ces_interop: Interoperates with other CES servers, Uses OpenTransaction
   OAuth2 servers - allowing credits to move between exchanges.
 - ces_message: Customize default messages depending on the exchange.
 - ces_offerswants: Features simple offers and wants within exchanges.
 - ces_statistics: Show statistical data about the community economy in the
   site.
 - ces_summaryblock: A block with a summary of your account state.
 - ces_user: Adds several fields to the usual drupal user entity.

For a more comprehensive developer documentation see the documents in docs/
folders and also visit http://www.integralces.net.

=======

Links: 

Community Exchange Systems Wikipedia:
http://en.wikipedia.org/wiki/Community_Exchange_System

Software site:
http://www.integralces.net

Drupal project page:
https://www.drupal.org/project/ices

Documentation for developers:
http://docs.integralces.net

Demonstration site:
http://demo.integralces.net

Contact mail:
info@integralces.net
