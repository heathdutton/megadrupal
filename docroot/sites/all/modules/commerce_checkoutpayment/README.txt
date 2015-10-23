Checkout.Com (http://www.checkout.com/) Payment Gateway integration with Drupal.


Introduction
============
Checkout.com connects your website or application to all major credit cards as well as an expansive range of local payments, all through one simple interface.


Installation
============
In Admin interface, go to Modules > Commerce (contrib), enabled Checkout.com GW3.


Configuration
=============
Once Checkout.com GW3 module has been enabled, go to Store > Configuration > Payment methods and find Credit / Debit cards (Checkout.com) and enabled the payment method rules.

Click on the edit button to edit settings.

To configure your Checkout.com payment module,

Enter the private key and publishable key from Checkout.com. (These details will be provided to you by your account manager.)

Select The Transaction method.

Set gateway auto capture time. (Default 0).

Select Method type.

Webhook
=============
Webhook url: example.com/checkoutapi/process
Success url: example.com/commerce_checkoutpayment/success


Maintainers
===========
Current maintainers:
* Jason Footing  - https://www.drupal.org/user/3217125/