======= SUMMARY =====

Integrates Instamojo Payment Gateway with Drupal Commerce.

=========== REQUIREMENTS ============

* Commerce Kickstart 7.x - https://drupal.org/project/commerce_kickstart

         OR,

* Drupal 7.x - https://drupal.org/project/drupal
     - commerce module
     - commerce_payment module
     - commerce_ui module
     - commerce_order module

================= INSTALLATION ====================

1. The module MUST be placed in /sites/all/modules so that the PATH to

   module will be: [DRUPAL-ROOT-FOLDER]/sites/all/modules/commerce_instamojo

   Enable the module.

2. Click on configure link next to the enabled module or browse to:

   http://yoursite.com/admin/commerce/config/payment-methods

3. a) Enable - Instamojo Payment Gateway - payment method rule.

   b) Click on edit

   c) In the Actions - click on edit next to - Enable payment method:

      Instamojo Payment Gateway

   d) Get your merchant account from Instamojo Payment Gateway. Enter following details in Payment Settings
	    Api Key
			Auth Token
			Virtual Payment Client Url (For this create payment link in Instamojo with pricing option as 'Pay What You Want' with minimum amount. Use the new Payment Client URL in this field.)
			Site URL (This is your site URL without last slash)
			Payment Api Url

 [Note: Site URL must be updated with your current drupal site URL (with NO trailing slash "/")]

4. Clear cache - http://yoursite.com/admin/config/development/performance.

5. Browse to Drupal permission page and allow Anonymous as well as Authenticated users to use Instamojo payment gateway

6. Visit your payment URL page in Instamojo. Click to "Advance Settings" section on payment URL page At the Payment Gateway Set Return url as
   http://yoursite.com/response_page
   Replace yoursite.com with your site URL
