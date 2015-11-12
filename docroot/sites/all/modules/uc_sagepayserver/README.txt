

=== About Ubercart Sagepay Server module ===

This module is intended to be used with Ubercart 2.x, to provide a simple method of integrating with Sagepay's (formerly Protx) Server payment transaction protocol (http://www.sagepay.com/integrating_sagepay.asp#server).

To use this module you will need a Sagepay vendorname.

Please review and report any issues.


=== About the Authors ===

Original module: Leo Pitt - leopitt on drupal.org

Initial Conversion for Ubercart 3.x on Drupal 7: Colin Thompson - aireworth on drupal.org


=== Installation instructions ===

1)

Download and unpack the module code into /sites/all/modules/ within your drupal site. You should now have a folder, /sites/all/modules/uc_sagepayserver, containing the module files.

2)

If you have previously installed uc_sagepaygoserver ("Sagepay Go Server"), then this module will replace it . Go to /admin/build/modules and disable uc_sagepaygoserver ("Sagepay Go Server"). Settings from that module will be automatically copied across to this new one in the next step, although you will still have to enable uc_sagepaygoserver at /admin/store/settings/payment/edit/methods.

3)

Enable uc_sagepayserver.

4)

Go to /admin/store/settings/payment/edit/methods to configure the module.

Click on the "Sagepay Server settings" to expand the module's payment method settings.

* Transaction Registration Mode: Pick Simulator, Test, or Live.
* Vendor Name: Enter in the vendor name provided to you by Sagepay
* Show debug info in the logs for payment notifications: Check this box if you want debugging info to be written to Drupal's log.

Enable the Sagepay Server payment method, and Save Configuration.


=== FAQ ===

Q: Where can I see data relating to the Sagepay Server transaction of the completed order?
A: When orders have been completed, the relevant Sagepay Server transaction details will be logged as an admin comment in the order.