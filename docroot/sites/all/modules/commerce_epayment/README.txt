DESCRIPTION
===========
ePayment (http://www.epayment.ro) integration for the Drupal Commerce payment and checkout system.

INSTALLATION INSTRUCTIONS FOR COMMERCE EPAYMENT
===============================================

First you should make sure you have an ePayment Merchant account, and are ready to configure it:

Installing & configuring the ePayment payment method in Drupal Commerce
-----------------------------------------------------------------------
- Enable the module (Go to admin/modules and search in the Commerce (Contrib) fieldset).
- Go to Administration > Store > Configuration > Payment Methods
- Under "Disabled payment method rules", find the ePayment payment method
- Click the 'enable' link
- Once "ePayment" appears in the "Enabled payment method rules", click on it's name to configure
- In the table "Actions", find "Enable payment method: ePayment" and click the link
- Under "Payment settings", you can configure the module:
  * Instant Payment Notification (IPN) URL: use this URL to configure the IPN settings in the ePayment Control Panel (more info below)
  * ePayment account: define whether to use the test parameter
  * ePayment Merchant ID: add your ePayment Vendor code
  * ePayment Key: add your ePayment Key coding
  * Currency code: select a currency
  * Language code: let ePayment know in which language the payment screens should be presented
  * IPN logging: define wheter to log full IPN notifications (used for debugging)


Configuring ePayment
--------------------
- Log in with your ePayment merchant account
- Go to Account management > Account settings > IPN settings
- Enter the URL copied from the "Payment settings" above (it should be something like: http://www.example.com/commerce_epayment/ipn)
- You can now process payments with ePayment!


Credits
=======
Andrei Mateescu (http://drupal.org/user/729614) @ Tremend Software Consulting (http://www.tremend.ro)
