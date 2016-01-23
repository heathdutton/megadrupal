RCCPagos Payment for Ubercart
=============================

This modules provides a payment method for Ubercart that allows you to use
RCCPagos Payment gateway in your Ubercart site.

Once you have enabled and configured the payment method in the site your users
will be able to select RCCPagos as a payment method in the checkout page. When 
the method is selected they can choose between one of the enabled RCCPagos
methods (RCCPagos allows payment with several credit cards and with 
imprimoYpago, a system that allows users to print a ticket and pay offline).
The user is redirected to an external page (RCCPagos) when confirming the order,
where he / she enters the information for the payment. No credit card information
is required or stored by this module. After finishing the process the user is
redirected back to the site. If everything went fine the order is updated and
marked as complete.

For imprimoYpago you can set the expiration time for the transaction (the time
the user has to make the payment). For the each of the credit cards you can
select the number of installments the user can choose from and the interest to
apply to each. The user will see the final price when selecting the installments
in the checkout page and in the checkout review page will also see the total
interest.


Installation
------------
Just extract the downloaded file to your modules directory and enable from the
admin interface.

Settings
--------
Once the module is enabled go to Store > Settings > Payment Methods and enable
RCCPagos. Then go to the method settings and set your account number and secret
key and enable the desired payment methods. For each selected method you must
select the number of installments and interest you want to allow.

Limitations
-----------
RCCPagos doesn't provide notifications, so when selecting imprimoYpago the order
will remain as RCCPagos pending.

Requirements
------------
* Ubercart payment [0].

[0] http://drupal.org/project/ubercart
