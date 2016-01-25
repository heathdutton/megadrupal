Module: Commerce Eurobank
Author: Yannis Karampelas - yannisc - netstudio.gr


Description
===========
Adds a payment method to Drupal Commerce to accept credit card payments through
the Eurobank (Greek bank) API via XML (not redirection).

Provides options to select the live or the test Eurobank environment and also to
have money authorized only or authorized and captured.

Works with euros, dollars and pounds. If there is need for more currencies,
please contact me sending me the currency code and I'll add them.


Requirements
============

* Commerce



Installation
============

* Copy the 'commerce_eurobank' module directory in to your Drupal
  sites/all/modules directory as usual.

* Enable the module from the Modules > Commerce Payments section


Setup
=====

* Go to Commerce Admin Menu > Payment Methods and enable Eurobank API.

* Edit Eurobank API and select whether you want to operate in the test or the
  live environment, input your username and hash password (provided by Eurobank)
  and select whether you want transactions to get authorization only or get
  authorization and capture amount as well.


History
=======

First beta1 release: 13/6/2011
