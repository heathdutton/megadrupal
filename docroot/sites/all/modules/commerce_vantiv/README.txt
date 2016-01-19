This module provides integration for drupal commerce with Vantiv payment gateway. The module provides two types of payment methods eCheck payment method – this will allow your customers to use echecks and the direct payment method. This module provides number of environments to test or integrate directly with your store.  It is recommended that you should start testing first with Sandbox environment and then use the Pre-live/Production environments. 
The module provides integration with Card on file, which will allow you to store credit card information for your customers without violating the PCI compliant.
Other features of the module are 3D-Secure, Auth-capture, and MOTO.
With 3D-Secure you can prompt your customers to use only 3D-Secure cards. The auth-capture provides you with the option to capture the transactions immediately or leave that for later, which can save you money if refund must be issued. The MOTO feature gives you the possibility to create transaction if the customer contacts you over the phone and wants to purchase a product from your site. The module also provides a fraud detection.

REQUIREMENTS
------------
The module requires mostly the drupal commerce core modules, which are
included in the commerce package.
This module requires the following modules:
  * Commerce (https://www.drupal.org/project/commerce)
  * Commerce Payment (https://www.drupal.org/project/commerce)
  * Commerce Payment UI (https://www.drupal.org/project/commerce)
  * Commerce Checkout (https://www.drupal.org/project/commerce)
  * Commerce Cart (https://www.drupal.org/project/commerce)
  * Commerce Order (https://www.drupal.org/project/commerce)
  * Commerce Order UI (https://www.drupal.org/project/commerce)
  * Commerce Customer (https://www.drupal.org/project/commerce)
  * Views (https://www.drupal.org/project/views)
  * Rules (https://www.drupal.org/project/rules)
  * Entity (https://www.drupal.org/project/entity)
  
RECOMMENDED MODULES
-------------------  
  * Commerce Card on file (https://www.drupal.org/project/commerce_cardonfile):
    If card on file functionality will be used card on file module 
    should be installed.
  * Commerce Recurring(https://www.drupal.org/project/commerce_recurring)
  * Commerce Price (https://www.drupal.org/project/commerce): 
    Will allow adding price field to products.
  * Commerce Product (https://www.drupal.org/project/commerce):
    Allows you to create products in your eCommerce site.
  * Commerce Product UI (https://www.drupal.org/project/commerce):
    Allows you to access the admin UI of the commerce_product module.
  * Commerce Product Pricing (https://www.drupal.org/project/commerce):
    Provides price management.
  * Commerce Product Reference (https://www.drupal.org
/project/commerce):
    Provides the possibility of creating a node that will reference a
    product.

INSTALATION
-----------------

1.  Download the module commerce_vantive from: https://www.drupal.org/project/commerce_vantiv
2.  Place the module in your_site/sites/all/modules/
3.  Enable the module from your_site/admin/modules
4.  If you don’t have an account and credentials for vantiv please visit:
http://www.vantiv.com/ and sign up.
5.  After that go to your_site/store/payment_methods
6.  Enable the vantiv payment method and edit the rule.
7.  Enter your credentials for USER ID, Merchant ID and Password.



CONFIGURATION
--------------------

1.  Set up your credentials in your_site/admin/commerce/config/payment-methods/manage/commerce_payment_vantiv_direct
2.  Choose whether or not you want to force your clients to use 3D secure
3.  Choose whether or not you want to use the fraud detection.
4.  If you installed already the commerce_cardonfile module 
You can enable that feature by checking the box for Enable Card on file functionality. 
5.  You can enable the eCheck payment method if you want to allow your customers to use echecks, this payment method also integrates with card on file functionality. 
6.  If you set your payment methods to use auth-capture you can use the auto-capture functionality to capture all pending transactions to do so, you have two options. Option number one is to go to your_site/admin/commerce/orders/batch 
7.  Here you can enter in the text field a date or any valid date expression like: now, -1 day, -2 days and so on. After pressing the Capture button this will trigger a batch process, which will capture all pending transactions in the specified period. If you place for example the word “now” in the text field the function will capture all pending transactions from this moment. If you place -1 day the function will capture all pending transactions from the day before. 
8.  Option number two for using the automated capturing process is to use the default rule created by the module (commerce_vantiv_capture_transactions_on_cron_run).
9.  You can capture transactions manually one by one by visiting your_site/admin/commerce/orders
10. You can issue a refund or void the transaction by visiting your_site/admin/commerce/orders

11. You can modify the rule to your needs, currently the rule is set to be triggered on cron run and to capture all pending transactions from now.
12. If you use Recurring products you can install the commerce_recurring module which will enable that feature for commerce_vantiv.
13.  You can use the MOTO feature by going to your_site/admin/commerce/orders
14. Here you can select an order and create a payment for that order by clicking Add payment button and choosing the payment method you want to apply: Vantiv direct or echeck.
15. Place the customer information and click Save.
16. This module provides an option to add manually card on file by going to your profile page click on Stored cards and click on Add card/echeck link. 
17. Select the payment method for credit card or echeck.
18. Enter card holder details and click Add card/echeck button.
