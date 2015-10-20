- SUMMARY --

Commerce Wepay integrates Wepay payment gateway with Drupal Commerce.
This module will add a new payment method Wepay
payment under payment methods settings(admin/commerce/config/payment-methods).

With the Commerce WePay you can instantly enable your users to accept credit
cards. The WePay supports a seamless user-experience for both buyers and
sellers on your platform, while shielding your application from fraud and
regulatory complexity.

Using Commerce WePay you can enable your users to accept
payments on your site, you can also charge your registered user for every
payment they receive using app fee, Similar like Google play store charges
30% of application price to developer (NOTE : App fee functionality is not
inclided in current 7.x-1.x branch, this is Work In Progress and hopefully will
be available soon).

IMPORTANT NOTE : Wepay is currently only for US citizens who have
US bank account and SSN(Social Security Number).

For more information go to https://www.wepay.com

-- Features 7.x-1.x --
* 7.x-1.x branch supports payments for Small business.
* Allow payments using Credit card, Bank Account, Wepay account.
* Uses redirect method for payment.

-- Future Roadmap --
* Support for Wepay Platform related apis.
* Possible Use cases for marketplaces, crowd funding & business tools.
* Allow your site users to get payments.
* Register account for Wepay user from Drupal application itself.
* Better error codes and error messages.
* Subscriptions

-- REQUIREMENTS --

* Drupal commerce: http://drupal.org/project/commerce
* commerce_payment module
* commerce_ui
* commerce_order
* Libraries API Modile

-- INSTALLATION and Configurations --

* Install as usual, see http://drupal.org/node/70151 for further information.
* Download wepay.php from https://github.com/wepay/php-sdk github repository
and put it in /sites/all/libraries/wepay/wepay.php
* Register on Wepay website - https://www.wepay.com
* From your login create account(Don't confuse this with Wepay site Register),
After login you will have option to create account which will be used to
configure Commerce wepay.
* In your Drupal site go to Payment method section
admin/commerce/config/payment-methods and enable Wepay payment.
* Now Edit "Enable payment method: Wepay Payment" Action inside rule.
* Fill all required fields in this Action configuration. Refer your account
created in Wepay website.
* Now you will be able to see Wepay payment while checkout on your site.
