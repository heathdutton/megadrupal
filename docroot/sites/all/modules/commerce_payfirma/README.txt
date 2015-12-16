
-- SUMMARY --

The Commerce Payfirma module is a payment module for http://payfirma.com

Payfirma uses javascript to "tokenize" credit card information in the browser.  
Then token information is sent via cURL POST rather that the actual card 
information, to add additional security and easier PCI compliance.

API Documentation: http://developers.payfirma.com/documentation

-- REQUIREMENTS --

* Drupal Commerce
* cURL
* SSL required by Payfirma to use their payment gateway.  If SSL is not 
installed module can only be used in "Test Mode".


-- INSTALLATION --

* Install as usual, see https://drupal.org/node/895232 for further information.


-- CONFIGURATION --

NOTE: Payment Pane should be configured to appear on the REVIEW phase of 
checkout.  See https://drupal.org/node/2249047

Navigate to /admin/commerce/config/payment-methods and enter your Payfirma 
Merchant ID, API key, and Public Key.  You may also select some basic options.

Test Mode will be locked if you do not have SSL installed on your server.


-- CONTACT --

Current maintainers:
* Calvin Tyndall (sleepingmonk) - https://drupal.org/user/53471

This project has been sponsored by:
* Payfirma
  Mobile, in-store and online payments
  built for business.
