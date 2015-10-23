Introduction
============

CheckPay PostNL becomes available as a Drupal Commerce payment method.
PostNL offers the CheckPay payment method as an afterpay variant.
The customer pays at checkout directly to PostNL. When delivering the package,
the customer receives a code by SMS which can be used to verify the delivery.
The money will then be sent to the merchant.


Installation
=============

1. Install the PHP SOAP and OpenSSL library on your server.
2. Enable the module.
3. Go to admin/commerce/config/payment-methods.
4. Enable payment method.
5. To configure: edit the payment method and then edit the Action
   to see the Payment Settings. All config fields should be visible.


Configuration
=============

1. Register at PostNL for using CheckPay.
2. Add the certificate files to a secure directory on your server and fill in
   the paths in the Payment Settings (see Installation step 5).
3. CheckPay Endpoint: Should be okay by default, but it can be changed
   if necessary.
4. Transaction Client Nr: This is a numeric account number you receive
   from PostNL.
5. Transaction Country: The country code of the CheckPay transaction.
6. Transaction Language: The language (fully written in English) of
   the CheckPay Transaction.
7. PSP: In case there is a Payment Service Provider as an intermediary, fill in
   the PSP code to identify it. Otherwise leave empty.
8. PSP Client Nr: In case there is a Payment Service Provider as
   an intermediary, fill in your client number as registered at this PSP.
   Otherwise leave empty.
9. Order description for the customer: This is a recognazible short description
   of the order. The customer can see this.
10. Extra charge: you can display a message about an extra charge for using
    this payment method. Configuring the actual adding of the charge
    is not included in this module.
11. The payment method is ready for use.


Contact
=======

Current maintainer:
* Lennart Van Vaerenbergh (lennartvv) - https://drupal.org/user/1338426
