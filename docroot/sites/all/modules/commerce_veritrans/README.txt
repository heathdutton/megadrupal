-- SUMMARY --
Commerce Veritrans[Drupal Commerce](https://drupal.org/project/commerce), module integrates with Veritrans(https://www.veritrans.co.id/) payment gateway into your Drupal Commerce shop.

Veritrans Brief:
=================
Veritrans provides 2 methods : VT-Web and VT-direct.
VT-Web :  Payment will be redirected to the payment page, Veritrans Style and interface kits have been provided Veritrans
VT-direct : Payments will be processed on the Merchant side and Merchant has full control over the style and interface.

We supports VT-Web method. VT-direct and Recurring payment support will come soon.

Resources:
=============
http://docs.veritrans.co.id/welcome/welcome.html

-- REQUIREMENTS --
To use this module, you need to signup as a merchant with Veritrans.
For more information, please visit https://www.veritrans.co.id/product.html
Once you are Merchant, you will receive a Merchant Id, Client and Server Key, which you will require while configuring, your Drupal commerce store.

-- INSTALLATION --
The <a href="https://github.com/veritrans/veritrans-php">Veritrans PHP Library</a> should exist in a directory named "veritrans" under "libraries"

* Install as usual, see http://drupal.org/node/70151 for further information.
* To change Merchant Id and Payment Mode(Test Transactions or Live Transactions), Go to Store settings > Payment methods admin/commerce/config/payment-methods.
* Look for Veritrans from available payment methods and click edit link.
* Under "Enable payment method: Veritrans" Action, click edit link. 
* Enter Server Key and Select Payment mode as Live transactions for Veritrans to accept real payments.

-- CONTACT --
https://www.drupal.org/u/nitesh-pawar
