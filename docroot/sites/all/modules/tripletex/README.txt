$Id: $

Description
-----------
The Tripletex module provides an interface to any Drupal module wanting to exchange information with the online accounting system Tripletex (http://tripletex.no)
Features include creating customer, creating invoice and have it sent to the customer and checking payment status of the invoice.
After installation and configuration you will find a form to create an invoice on the admin/content/invoice/add path and also on the user/x/invoice/add path - x being the user id (uid).
In addition to this you will also find a list of all your invoices at admin/content/invoice if you have views installed.

Setup
-----
Howto set up the system

To connect this module to your Tripletex account, You have to request Fakturaimport to be enabled by Tripletex.
If you want invoices to be sent to the customer upon generation, you have to navigate to Faktura->Fakturainnstillinger and set the checkbox "Send importerte fakturaer på e-post:" under the Fakturaimport section.

Modules
-------
The module is packaged with the following modules individually installable:
Tripletex:          Api to the on-line accounting system Tripletex. No dependencies
Tripletex Pay:      Interface to create a payment method for the Pay API
Tripletex Signup:   Interface to generate electronic invoice and send this to a user that signs up to a node. Requires Signup Pay for a Node (http://drupal.org/project/signup_pay) and Signup (http://drupal.org/project/signup)
Tripletex Uc:       Ubercart payment integration where an electronic invoice is sent to the customer and where payments are tracked in order status.
Tripletex Commerce: Electronic invoice payment option and payment status tracking for Drupal Commerce 

Tripletex Dummy API: Dummy JSON API mimicing the real API for testing purpouses. Do not enable normally.