Description
===========
Commerce Everyday integrates Everyday online payment method 
(Everyday-verkkomaksu) with Drupal Commerce. 

Everyday is a Finnish billing and part payment online gateway provider.
See more at http://www.everyday.fi/verkkomaksu/

Istallation instruction for Commerce Everyday Web Payment 
=========================================================
1. Install Commerce Everyday module as usual

2. Enable Commerce Everyday payment method 
   (admin/commerce/config/payment-methods)

3. By default module is in test mode (with test mode credentials provided)

4. Configure Everyday settings 
   (admin/commerce/config/payment-methods/manage/commerce_payment_commerce_everyday/edit/3). 
   You'll need to create Everyday-verkkomaksu storekeeper account and 
   obtain a customer ID and secrect key via 
   https://kauppias.opr-vakuus.fi/sopimuspohja.php. 
