DESCRIPTION
===========
Commonwealth bank, aka CBA (https://www.commbank.com.au/)
integration for the Drupal Commerce payment and checkout system.

INSTALLATION INSTRUCTIONS FOR COMMERCE COMMONWEALTH
=================================================
1. Create a merchant account and set up commweb.
https://www.commbank.com.au/business/merchant-services/accept-online-payments/
 \ commweb.html
You will be given a merchant ID.

2. Your details will be forwarded from CBA to commweb, and you will be assigned
   an access code.

3. Visit the merchant centre and create the hash secret.

4. Install and enable commerce_commonwealth & commerce_commonwealth_3party_vps
   as usual.

5. Enable the payment 3-party VPS method at /admin/commerce/config/payment-methods

You can find a detailed guide on how to configure payment methods at
http://www.drupalcommerce.org/user-guide/payments

6. Configure the payment gateway with the settings received from your merchant
   adviser and the merchant centre.

7. Start handling payments.
