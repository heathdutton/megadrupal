COMMERCE SUOMEN VERKKOMAKSUT README
===================================
This module integrates Suomen Verkkomaksut payment method with 
Drupal Commerce.

INSTALLATION
============
- Enable the module as usual.
- You will probably need to enable more of the Commerce modules to make your
  shopping cart work.
  
CONFIGURATION
=============
Configuration page is located at
admin/commerce/config/payment-methods/manage/commerce_payment_commerce_suomenverkkomaksut

S1 TYPE
=======
If using S1 type, no billing information is needed. This type does not require
any additional configuration.

E1 TYPE
=======
If using E1 type, billing information is required. The customer's first and 
last name must be in two separate fields. Configure this by going to 
admin/commerce/customer-profiles/types/billing/fields/commerce_customer_address
and uncheck "Name (single line)" and check "Name (First name, Last name)".
