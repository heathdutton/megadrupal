Commerce Payment Network module

Payment Network AG integration for the Drupal Commerce payment
and checkout system.

Dependencies
============

Drupal 7.

Install and Configure
=====================

1) Copy the Commerce Payment Network folder to the modules folder in your
   installation.
   Usually this is sites/all/modules.
   Or use the UI and install it via admin/modules/install.

2) Payment Network AG account

   Payment Network AG: http://www.payment-network.com

   Make sure you have an Payment Network Merchant account and created a project:

   Configuring Payment Network
   ---------------------------
   - Log in with your Payment Network AG account.
   - Under the Basic Settings go to Interface and configure
     'Success link' and 'Abord link'.

     * Success link: http://www.your-domain.com/checkout/-USER_VARIABLE_0-/
     payment/return/-USER_VARIABLE_1-?status=-STATUS-&hash=-USER_VARIABLE_0_
     HASH_PASS-&transaction=-TRANSACTION-

     * Abord link: http://www.your-domain.com/checkout/-USER_VARIABLE_0-/
     payment/back/-USER_VARIABLE_1-

   - Choose 'Automatic redirection' if you would.
   - Go to Your Projects and choose your project.
   - Go to Extended settings > Passwords and hash algorithm and set up
     your project and notification password.
   - For Input check use SHA1 as Hash algorithm.
   
   - For test payments:
     It is possible to test payments if you enable the "Test" - Checkbox
     in your Payment Network AG project account.

3) Configuring the Payment Network payment method in Drupal Commerce
   -----------------------------------------------------------------
   - Enable the module (Go to admin/modules and search in the
     Commerce (Contrib) fieldset).
   - Go to Administration > Store > Configuration > Payment Methods
   - Under "Disabled payment method rules", find the
     Payment Network payment method.
   - Click the 'enable' link.
   - Once "Payment Network" appears in the "Enabled payment method rules",
     click on it's name to configure.
   - In the table "Actions", find "Enable payment method: Payment Network"
     and click the link.
   - Under "Payment settings", you can configure the module:
     * User ID: Your User ID from Payment Network
     * Project ID: Your Project ID from Payment Network
     * Project password: Your Project Password from
       Payment Network > Extended settings > Passwords and hash algorithm
     * Notification password: Your Notification Password from
       Payment Network > Extended settings > Passwords and hash algorithm

   - You can now process payments with Payment Network.

Author
======
Tobias Christian (http://drupal.org/user/35369) of forward-media.de
(http://www.forward-media.de)
The author can be contacted for paid customizations of this module as
