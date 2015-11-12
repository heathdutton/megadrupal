About
=====
This module allows Ubercart to obtain shipping quotes from FedEx.  It
provides quotes of FedEx shipping costs to your customers during checkout,
and lets your customers select FedEx services from among the shipping choices.
Advanced, optional functions implemented by this module include verification
of a customer's shipping address by FedEx and generation of FedEx barcoded
shipping labels.

Administrators may specify which FedEx services to quote, and may choose
various options regarding packaging, pickup/dropoff, insurance coverage,
etc.  The module will contact the FedEx server and submit, retrieve, then
display quotes on the checkout page when the customer clicks on "Calculate
Shipping Rates" or when a shipping address is entered.

A function that lets you track packages and display the tracking information
is included in the source code - this function is not interfaced into the
store menus, it is provided merely for reference.  A completely integrated
tracking solution for FedEx, UPS, and USPS may be found at
http://drupal.org/project/uc_tracking.

This module still has limitations and some hardwired values, which I've tried
to document here and in the code.


Quick Start
===========
Check requirements:  PHP 5 built with --enable-soap and --with-openssl

Obtain developer's credentials from FedEx.  See "Before You Begin" below
for more details how.

Disable any previous version of uc_fedex.  Copy this tarball into your
sites/all/modules directory and unzip/untar it.

In your web browser, navigate to admin/build/modules and enable the FedEx
module.  Run update.php.  Go to admin/store/settings/quotes/methods/fedex
and enter the required information.  Finally, enable FedEx quotes at
admin/store/settings/quotes/methods

If this doesn't work, read the rest of this document (which you really
should have done first, anyway!).


Features
========
The quotes returned from the FedEx server are based on the store ZIP code
for the origination address and customer ZIP code (or country and zone, in
the case of non-US customers) for the destination address.  THE STORE ADDRESS
MUST BE SET! Check that now, I'll wait... (See admin/store/settings/quotes/edit
for store address settings).  Products are divided into packages in order to
keep the total package weight below the FedEx weight limit (150lbs).  Quotes
presented to the customer reflect the total shipping cost for all the packages
in an order.

An admin menu option lets you chose a "Weight Markup" to be applied to every
order - this can adjust the order total weight based on a percentage, a
multiplier, or an addition, and is meant to account for the additional weight
of your packing materials.  A rate markup is also provided, to adjust the
shipping rate based on a percentage, a multiplier, or an addition. The rate
markup is used to compensate for handling and other expenses you may incur
that you want to lump in with the shipping cost.

The admin has the option of specifying Residential or Commercial quotes.
Shipping using the FedEx residential service costs a little more.  Shipping
to a residential address using the *commercial* service costs more plus
there's an added penalty.  So if you ship mainly to residences (whether
there is a business at that residence or not), use the Residential quotes
(default).  If your customers are mostly or entirely commercial addresses,
you may want to use the commercial quotes to present a slightly lower rate to
your customer.  Note that this selection does not actually affect the amount
you pay to FedEx for the package, but it DOES affect how much you collect
from your customer for shipping charges.  Optionally, the FedEx Address
Validation service may be used to automatically determine the Residential/
Commercial status of the customer's shipping address.  Address Validation
is classified by FedEx as an "advanced" service, which requires additional
permission from FedEx, over and above the normal permissions granted when
you obtain web service credentials.

Tracking information may be obtained using the uc_fedex_tracking_request()
function.  An example of how to use this function is in the code comments.
A completely integrated tracking solution for FedEx, UPS, and USPS is
provided by the uc_tracking module, which may be found at
http://drupal.org/project/uc_tracking.


Requirements
============
PHP 5 built with --enable-soap and --with-openssl is REQUIRED for this
module to work.

The FedEx Web Services API uses SOAP over HTTPS for communication with the
FedEx Server.  SOAP is a standard extension to PHP 5.


Before You Begin
================
Rate quotes won't work until you obtain FedEx developer credentials.  These
credentials are easy (and free!) to obtain - you simply have to register on
the FedEx web site and you will have them in minutes.  FedEx credentials
consist of a Key, Password, Account Number, and Meter Number, and need to be
entered in the admin form at admin/store/settings/quotes/methods/fedex once
you have installed the module.  There are two types of credentials - Test and
Production.  You will need to get your Test credentials first, before you
will be issued Production credentials.

Test credentials allow you to make transactions on the FedEx Test Server.
I strongly suggest you do all your development on the Test server and move to
the Production server only when ready to go live.  While this module does NOT
make any transactions that will incur charges to your FedEx account, testing
can generate a lot of bogus and perhaps invalid transactions that are best
kept off the production machine.  Remember, YOU are responsible for your
FedEx developer credentials - if this module causes problems on the production
server YOU will be the one contacted.  I think we would both prefer that the
bugs were worked out on the Test Server!

Your Test credential Account Number is *not* the same as your normal FedEx
Shipper Account Number; because of this, any discount rates associated with
your Shipper Account will not be reflected in the rate quotes returned by the
Test server.  (The Production server, however, uses your normal Account Number
and *will* provide both list rates and discount rates.)

To obtain your FedEx Test Credentials, first register at:
http://www.fedex.com/us/developer/ Then navigate to:
https://www.fedex.com/us/developer/wss/develop.html and
fill in the form.  Your credentials should be presented to you immediately
followed by the same information split into two separate e-mails.  Also
included will be instructions for obtaining your Production credentials,
when you need them.

Address Validation and Label Printing (Ship Service) are both considered
advanced services that require additional permissions.  If you want to
use one or both of these services you will need to call FedEx Technical
Support after obtaining your credentials to have them enable permissions
for these services.  The phone number may be found on the FedEx site at
https://www.fedex.com/wpor/web/jsp/drclinks.jsp?links=techresources/support.html
These permissions are granted separately for the Test and Production servers.


Installation
============
Before you use this module, disable any previous version of uc_fedex,
then remove that code from your machine.

Copy the tar.gz archive for this module into your sites/all/modules directory
and unzip/untar it.

In your web browser, navigate to admin/build/modules and enable the FedEx
module.  Run update.php.  Go to admin/store/settings/quotes/methods/fedex
and enter the required information.  Finally, enable FedEx quotes
admin/store/settings/quotes/methods

You should now be receiving FedEx quotes on your checkout page.  If you still
have problems see the "Troubleshooting" section below.


Limitations
===========
All packages are hardwired to be 1"x1"x1" - this is the size FedEx uses
on their own web page to deliver a "quick quote".  A more detailed quote
requires actual package dimensions.  FedEx defines the "dimensional weight"
(in pounds) of a package as length x width x height in inches, divided by 194
for shipments within US (for international shipments divide by 166).  For
the hardwired box size of 1"x1"x1", the "dimensional weight" is less than
1 ounce.  Because FedEx will charge based on the greater of the actual weight
and the dimensional weight, this hardwired box size ensures that the quote
presented to the customer ignores the dimensional weight and just returns
the rate based on your package weight.  For large but light objects, where
the dimensional weight should be used instead, you'll have to enter the
product weight a bit higher than actual - i.e. enter the dimensional weight
on the product page rather than the actual weight in order to receive
accurate quotes.

Drop-shipping will be included eventually, but for now everything is assumed
to ship from the store address.

This module works for sending packages from US to other countries. Quote
type MUST be set to ACCOUNT for international quotes to work.

I still have to decide how to handle the shipping date for the quote - it's
not as simple as entering the current day because, say, your customer chooses
"Next Day" delivery, but it's already after the shipping cutoff time - then
the ship date is NOT "today".  You don't want to be obligated to ship under
those conditions - I plan to add an admin config for the module that lets
you specify cutoff times and closed days for your store.


Address Validation
==================
The FedEx Address Validation Web Service can be used to determine whether a
U.S., Puerto Rico or Canada delivery addresses is Commercial or Residential.
FedEx does not support validation for any other location.  Address Validation
is a FedEx "advanced" service, which requires you to call FedEx and have your
developer access upgraded to allow you to use this service.  You will have
to do this even for both the Test server and the Development server separately.
If the checkbox "Let FedEx determine if an address is Commercial or Residential"
is selected at admins/store/settings/quotes/methods/fedex/edit, this module
will use the Address Validation Web Service to make the determination, and
default to your selection of Commercial or Residential if the Web Service
fails or times out.  Additionally, JavaScript validation of the address
entered by the customer on the checkout page can be performed if the
"Enable JavaScript validation of customer destination address at checkout"
checkbox is selected.


Label Printing
==============
This module currently supports created barcoded shipping labels only
for FedEx domestic (within the USA) shipping.

The Ship Service is used to generate FedEx barcoded labels that can be
used for shipping.  Label printing is a FedEx "advanced" service, which
requires you to call FedEx and have your developer access upgraded to allow
you to use this API.  Additionally, you will have to generate test labels
and physically mail these labels to FedEx for certification before you will
be allowed to access the Production server to print real labels.  FedEx
needs to ensure that your software (this module) AND your hardware (printer)
work together to generate labels that can be properly scanned by FedEx.

FedEx supports LASER printer labels and THERMAL printer labels. LABELS
PRINTED ON INKJET PRINTERS ARE NOT SUPPORTED AND WILL NOT BE APPROVED.
Color laser printers may be used, but in "black and white" mode only.

In order to generate labels, Ubercart must be configured to require
customers to enter their phone number at checkout.  You may set this
field as required at admin/store/settings/checkout/edit/fields

Labels that are generated in PDF format and printed with a laser printer
must not be scaled or resized. You must ensure that you print the document
at "actual size" from within your PDF viewer application instead of
"scaled to fit page".

When you generate labels in PNG image format, the image is rasterized at
200 DPI, which is the minimum resolution required by FedEx.  When you
view the image on your screen, most applications degrade the DPI to your
screen resolution. Typically, this is 96 DPI, which is far less than the
required minimum of 200.  To produce the label and barcodes in the required
DPI, you must scale (or resize) the image before printing.  How you scale
the image depends on the application you are using to view and print the label.
In the usual case of viewing/printing from a web brower, you may scale
the printed image from the "Page Setup..." menu.

Scaling Instructions for PNG Labels:

  * If your screen resolution is 96 DPI:
      Scale Factor = 96 ÷ 200 = 48%
  * If your screen resolution is 72 DPI:
      Scale Factor = 72 ÷ 200 = 36%
  * If your screen resolution is not 96 or 72 DPI
      Scale Factor = (screen resolution in DPI) ÷ 200


Troubleshooting
===============
Does your site have PHP 5 built with --enable-soap?  Execute <?php phpinfo();?>
to see the details of your PHP installation.  If SOAP has been enabled, you
should see a section of this report which lists the SOAP configuration
parameters.

Does your site have PHP 5 built with --with-openssl?  Go to
admin/reports/status/php or execute <?php phpinfo();?> to see the details
of your PHP installation.  If PHP has been built with OpenSSL, then you
should see SSL listed under "Registered Stream Socket Transports".

You did set your store address at both admin/store/settings/store/edit and
admin/store/settings/quotes/edit, didn't you?

Check to see that you have entered the correct developer credentials - Test
credentials for the Test server, Production credentials for the Production
server.  If you're trying to use Address Validation or Label Printing, make
sure you've taken the additional step of contacting FedEx to upgrade your
permissions.  If you receive an "Authentication Failed" message, you have
the wrong credentials or you don't have permission to use the service on
that server.  This is true even if you're "positive" you entered everything
correctly.

Check the box "Display debug information to administrators" on the form at
admin/store/settings/quotes/edit .  This will print debug information on the
checkout page, including the full request sent to the FedEx server and the
complete response.  Examine these lines carefully for any hints of what is
going wrong.

Read the comments in the code - there are some debugging print statements left
in that can be uncommented if you have problems, and there is a menu option in
the shipping quotes admin pane which lets you enable some other debug info.
