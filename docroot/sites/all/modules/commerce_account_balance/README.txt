CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* Roadmap
* Maintainers


INTRODUCTION
------------
The Commerce Account Balance is a module that enables a -per user- balance which
you can use for purchasing items or actions on the website.

The user has a 'virtual' account to which he can deposit real money. With the
deposited money he can purchase actions/items on the website, which are in fact
the execution of a custom Rule Set.

The process of depositing money on your account integrates with the Commerce
Module, allowing any type of payment and all the other stuff Commerce has to
offer. Transactions are saved as entities and can be accessed by views, linking
them to Commerce Orders and stuff.

Purchasing items using the deposited money on your account works with a
‘purchase’ field. The purchase field has an ‘amount’ element and a Rules Action
Set. Upon purchase the amount is taken out of the account and the Rules Action
Set is executed.

This module is particularly useful if you offer lot's of small services for
small amounts of money, for example granting access to a node, send an email
with secret information, or give access to a download. You don't want the user
to literally pay for each and everyone of them, since the user has to go
through the trouble of finding credit cards, etc. Besides the shop owner would
have to pay higher fixed fees for every transaction.

Better is getting to him pay a 'one-time-only' amount into is account, and use
that account to purchase these small 'actions'.

Websites that use this type of payment system are, for example, ShutterStock
and IconFinder.


REQUIREMENTS
------------
This module requires the following modules

* Commerce payment
* Commerce order
* Commerce Line Item
* Commerce Checkout
* Select or other
* Number (core)
* Rules
* Views

- Note: For configuration purposes it is recommended to activate Payment UI,
  Order UI and a payment method module, although they are technically not
  dependencies.

INSTALLATION
------------
* Download and activate the module and dependencies
* Create a field on any entity of the 'purchase' type
* Create a Rules Action Set with an action to be executed upon purchase. The
  action set needs the entity and the user as parameters, in any order.
* Create an entity (probably a node) where you fill in the price and the
  action to be executed.
* Activate a payment method that that fulfils payment immediately.
  Deposits will only work upon ‘fulfilment’ of the payment.

- Note 1. There is a Balance Block that you can put in any region, showing
  the current balance plus a link to the deposit page.

- Note 2. You can use any payment method, but remember that the Rules Action
  Set won’t be executed until payment is fulfilled. The Payment method Example
  doesn’t ‘fulfil’ the payment by default. You can create a rule to fulfil the
  payment upon checkout.

- Note 3. There is a default action you can use on a user that allows an
  administrator to deposit money to any user account. Use e.g. admin_views and
  views_bulk_operations and check the ‘deposit money’ action on the people page.

ROADMAP
-------
Implement option for Redeeming gift codes.


MAINTAINERS
-----------
* Teun van Veggel(nuez) -  https://drupal.org/user/758020

This project has been sponsored by:
* NUEZ WEB
  Madrid based company specialised in design and development for Drupal.
