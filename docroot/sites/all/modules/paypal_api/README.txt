;$Id$

PAYPAL API

Implements the Paypal API for Drupal sites.

With this module you may configure content types to require payment for creating or viewing.  You may further limit viewing to
payment per instance, or payment to view any instance of a given type.

Payments may be configured to be made either through a Pay Now button you create through your Paypal account, or by using
the Paypal API.

If you choose to use the Paypal API, you will need to store your Paypal account email in your Drupal web site.
Use of Pay Now buttons created through your control panel do not have this requirement.

Either way, the Paypal API module handles your IPN (instant payment notification) for you, and you may create custom code in your
own module to handle the payment notification (through hook_paypal_api_ipn()) or else query Paypal API as necessary.  See 
paypal_api.api.php in this package for more documentation.


PAYPAL BUTTONS OR API
You should use pre-configured Paypal buttons that you create yourself if the products you are selling have a fixed price and
you will not be adding discounts or changing prices according to user role or other variables.

You should use Paypal API buttons if you want to change the amount of the sale from transaction to transaction.

Author
--------------
Aaron Craig
aaron@evolving-design.com

Sponsors
--------------
Evolving Design (evolving-design.com)
Learning TV (learning-tv.com)