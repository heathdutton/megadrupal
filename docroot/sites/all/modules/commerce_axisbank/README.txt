-- SUMMARY --

Integrates AxisBank Payment Gateway with Drupal Commerce.

-- REQUIREMENTS --

* Commerce Kickstart 7.x - https://drupal.org/project/commerce_kickstart

         OR,

* Drupal 7.x - https://drupal.org/project/drupal
     - commerce module
     - commerce_payment module
     - commerce_ui module
     - commerce_order module

-- INSTALLATION --

1. The module MUST be placed in /sites/all/modules so that the PATH to

   module will be: [DRUPAL-ROOT-FOLDER]/sites/all/modules/commerce_axisbank

   Enable the module.

2. Click on configure link next to the enabled module and go to:

   admin/commerce/config/payment-methods

3. a) Enable - Commerce AXISBANK Payment Gateway - payment method rule.

   b) Click on edit

   c) In the Actions - click on edit next to - Enable payment method:

      Commerce AXISBANK Payment Gateway

   d) Get your merchant account from AXISBANK Payment Gateway. Enter the

      details in Payment Settings

 [Note: Site URL must be updated with your current drupal site URL
  (with NO trailing slash "/")]

4. Go to the Permission page and check permission for Commerce AXISBANK Payment Gateway MODULE.

5. Clear cache - admin/config/development/performance.
