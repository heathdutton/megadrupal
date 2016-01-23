CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------

Full featured module for Ubercart to accept Wallet one payments on
your website.

Features:

 * typical installation
 * ability to browse payments in admin interface and taking actions
 * on them(deleting and enrolling)
 * ability to allow or dissalow some wallet one payment methods
 * ability to select wallet one payment methods on your site
 * or on wallet one site
 * ability to select widget for wallet one payment methods list
 * protection against multi-enrolling
 * ability to set up (add,remove,rename) wallet one payments methods
 * in payment_methods.ini file
 * setting up this module as other ubercart payment module
   /admin/store/settings/payment

REQUIREMENTS
------------

This module requires the following modules:

 * Ubercart (https://www.drupal.org/project/ubercart)
 * Cart (uc_cart - ubercart submodule)
 * Payment (uc_payment - ubercart submodule)
 
INSTALLATION
------------

 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

 * Customize settings in Store » Configuration » Payment methods:
   - Your wallet one id, wallet one secret key, signature type, currency code
     must be equal with your Wallet one settings here
     https://www.walletone.com/merchant/client
   - Your wallet one integration setting: script url must be
	   http://your-site/uc_w1/done
   - Optionally, to allow customer select wallet one payment method
     on your site you must check "Select payment method on your site." option.
   - Optionally, you can select wallet one payment methods to use on you site
     from payment method list.
   - Optionally, if you don't want to use some wallet one payment methods
  	 you must check "Disable some payment methods." option
		 and select methods, you dont't want to use.

MAINTAINERS
-----------

Current maintainers:
 * Konstantin Klimov (moelius) - https://www.drupal.org/user/1823620
