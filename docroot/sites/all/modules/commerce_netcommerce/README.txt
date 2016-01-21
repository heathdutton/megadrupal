This module implements the NetCommerce payment service
(http://www.netcommerce.com.lb) for use with Drupal Commerce.

# NetCommerce Module Installation
1. Install Drupal Commerce.
2. Install Commerce NetCommerce module as normal.
3. Go to Store > Configuration > Payment methods, and click "enable"
   next the NetCommerce rule.
4. Click "edit" next the NetCommerce rule, then "edit" next to the
   Action "Enable Payment Method: NetCommerce"
5. Fill in your NetCommerce merchant number and SHA key, and select
   the payment mode (testing, real) that you want to use.
6. NetCommerce should now be available for checkout.

# NetCommerce e-Bill Installation
1. Install Drupal Commerce.
2. Install Commerce Recurring, and set up some recurring products.
3. Go to Store > Configuration > Payment methods, and click "enable"
   next the NetCommerce e-Bill rule.
4. Click "edit" next the NetCommerce e-Bill rule, then "edit" next to the
   Action "Enable Payment Method: NetCommerce e-Bill"
5. Fill in your NetCommerce merchant number and MD5 key.
6. At this point, NetCommerce e-Bill should be available for checkout, however
   you will probably want to restrict access to this payment processor
   to recurring products only. To do that:
7. From NetCommerce e-Bill rule edit form, add a new Condition by clicking
   "+Add condition".
8. Choose "Order contains a recurring product", choose the "commerce_order"
   in the data selector, and Save.
9. The e-Bill module only works with a single product per transaction. So,
   you should limit the availability of this product to cases with a single
   product. To do that:
10. Add another condition to the e-Bill rule: "Total product quantity
    comparison". Make sure it's set to equal to 1.
11. If you have non-recurring products on this same site, you will want to
    add a condition to all non-recurring payment systems to check that the
    order does not contain recurring products.
12. e-Bill creates new recurring orders without Drupal Commerce, so you will
    probably want to disable the rule titled "Generate recurring orders on cron run".