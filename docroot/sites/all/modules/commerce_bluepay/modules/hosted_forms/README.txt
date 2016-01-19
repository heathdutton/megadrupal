Contents
--------
 * Overview
 * Features
 * Requirements
 * Known Problems
 * Similar Projects
 * Development Notes


Overview
--------
This module adds BluePay Secure Hosted Forms as a payment option for Drupal 
Commerce.

BluePay’s hosted payment forms allow merchants to accept and process credit 
cards online through a Web form hosted on BluePay’s secure server.

Allows merchants to:
	Accept convenient and secure online payments with a reduced risk of liability.
	Cut paper billing costs with online recurring payment capabilities.
	Reduce PCI scope through BluePay’s secure, PCI compliant databases.

If you have a BluePay account, all you will need to do is configure this payment
method with some of your account information, choose your transacation types, 
and the cards you can accept, and Secure Hosted Forms will take care of the 
rest!


Features
--------
Integration to BluePay Hosted Forms as a payment solution.

Requirements:
* PHP5
* Drupal Commerce

Note: This module requires you to have a BluePay account in order to allow users
to checkout and pay with BluePay. Sign up at 
http://www.bluepay.com/get-started-today.


Known Problems
--------------
ACH notifications may take a long time to process.


Similar Projects
----------------
This project is similar to most payment options for Drupal Commerce.  The
primary difference is that it is specifically designed to work directly with
the BluePay Secure Hosted forms solution, and is the only module that does so.


Development Notes
-----------------
This project was sponsored by BluePay.

Please note that this module uses a different API than the regular BluePay
module. You can find the documentation relevant to the Hosted Forms at this
URL:

https://www.bluepay.com/sites/default/files/documentation/BluePay_bp10emu/BluePay%201-0%20Emulator.txt

This module was originally produced by Tallosoft.
