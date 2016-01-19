About
=====
This module allows Ubercart to obtain shipping quotes from Canpar. It
provides quotes of Canpar shipping costs to your customers during checkout,
and lets your customers select Canpar services from among the shipping choices.

Administrators may specify which Canpar services to quote, and may choose
various options such as markups and insurance. This module will contact the
Canpar server and submit, retrieve, then display quotes on the checkout page
when the customer clicks on "Calculate Shipping Rates" or when a shipping
address is entered.


Quick Start
===========
Check requirements:  PHP 5 built with --with-openssl

If you want to use custom rates you need to obtain a user token from Canpar.

Disable, uninstall, and delete any previous version of uc_canpar.
Copy uc_canpar.tar.gz into your sites/all/modules directory and unzip/untar it.

In your web browser, navigate to admin/build/modules and enable the Canpar
module. Run update.php. Go to admin/store/settings/quotes/methods/canpar
and enter the required information. Finally, enable Canpar quotes at
admin/store/settings/quotes/methods

If this doesn't work, read the rest of this document (which you really
should have done first, anyway!).


Features
========
The quotes returned from the Canpar server are based on the store postal code
for the origination address and customer postal code (or country and postal
code, in the case of customers outside of Canada) for the destination address.
THE STORE ADDRESS MUST BE SET! Check that now, I'll wait...
(See admin/store/settings/quotes/edit for store address settings).

An admin menu option lets you chose a "Weight Markup" to be applied to every
order - this can adjust the order total weight based on a percentage, a
multiplier, or an addition, and is meant to account for the additional weight
of your packing materials. A rate markup is also provided, to adjust the
shipping rate based on a percentage, a multiplier, or an addition. The rate
markup is used to compensate for handling and other expenses you may incur
that you want to lump in with the shipping cost.

This module allows you to display "base" rates or "custom" rates. Base rates
are Canpar's list rates and do not include fuel surcharges or taxes. Custom
rates are the discounted, negotiated rates specific to your business. Custom
rates DO include the fuel surcharge and taxes. You must have a Canpar account
and shipper number to use custom rates, and must obtain a Canpar user token.
If you don't have a shipper number or user token, custom rates may be tested
using the shipper number 99999999 and the user token CANPAR.


Requirements
============
OpenSSL is required for custom rates, because HTTPS is used.


Troubleshooting
===============
Does your site have PHP 5 built with --with-openssl? Go to
admin/reports/status/php or execute <?php phpinfo();?> to see the details
of your PHP installation. If PHP has been built with OpenSSL, then you
should see SSL listed under "Registered Stream Socket Transports".

You did set your store address at both admin/store/settings/store/edit and
admin/store/settings/quotes/edit, didn't you?

Check the box "Display debug information to administrators" on the form at
admin/store/settings/quotes/edit . This will print debug information on the
checkout page, including the full request sent to the Canpar server and the
complete response. Examine these lines carefully for any hints of what is
going wrong.
