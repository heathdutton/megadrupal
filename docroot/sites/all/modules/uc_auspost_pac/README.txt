Ubercart Australia Post Postage Assessment Calculator.

Based on the PAC PCS Dev Centre API version 1.3
http://auspost.com.au/devcentre/

State of the Module
===================

Module has reached 1.0-beta1 release for testing and feedback. All changes to
API and variables will be handled by hook_update_N functions in dev and full
releases.

The beta releases may contain bugs, or may work perfectly. Dev snapshops may
not work at all, or again, may work perfectly. Please review any changes
listed in the CHANGELOG.txt, and the git commit logs.

If you experience any issues updating between dev snapshots, please uninstall
and re-install the module before reporting them as issues.

The module also has some functionality limitations which may make it
unsuitable for use in your site.

- Shipping Options
Only mutually exclusive options are available for generating quotes.
Sub-options such as Extra Insurance and Delivery Confirmation are not
included. There are plans to include these with both admin configured static
options, and also end user input.

Obtaining an API key
====================

You will need an API key for this module to function.

1. If you do not already have an Australia Post ID, create one on the
   Australia Post website.
   https://id.auspost.com.au/csso/customer/registration?execution=e1s1

1A. Confirm your account via the email sent the email address you provided.

2. Enter your name and email address from your Australia Post ID into the API
   application form in the Australia Post Development Centre.
   https://auspost.com.au/devcentre/pacpcs-registration.asp

3. Your API Key will be sent to your Australia Post ID email address.

NB At time of writing, the server sending the API Key is not listed in the
auspost.com.au SPF record, and the record is set to exclude all servers not
mentioned.

Configuring the module
======================

1. Install the module to either your global (sites/all/modules), or site
   specific (sites/<sitedir>/modules) modules directory.

2. Obtain an API Key as directed above.

3. Navigate to the module configuration page and enter the API Key. 
   Admin -> Store -> Configuration -> Shipping quotes -> Aus Post

4. Return to the Shipping quotes methods and enable the methods required.

5. Edit each method to choose which accessorials will be available to the
   client.

6. Choose and configure a packing method for each shipping method on their
   settings pages.

NB The default shipping source and/or each product node's shipping address
must be set with at least the postcode for this module to work. Valid
dimensions for all products must also be set.

Packing Methods
===============

To be able to quote via the AusPost PAC API, all of the products in the cart
must be grouped into parcels with a packing algorithm. Please see the README
in the packing directory for more details.

There are two packing algorithms provided in this module; Do Not Pack, and
Volume and Weight. The "Packaging" module has been brought to my attention and
it may be investigated to provide an alternative to implementing additional
strategies, or separating out the Packing API into a stand-alone module.

You may supply your own Packing algorithm by hooking into the Packing API
which is detailed uc_auspost_pac.packing.api.php.

Glacial Cache - Long term cached data
=====================================

There is a lot of information which is provided by the API that will rarely,
if ever, change. This information is kept in a cache table that is not part of
the normal flushing regimen. In the event that it needs to be rebuilt, a
manual flush is required by clicking on Flush Glacial Cache on the Aus Post
settings page.

An updated set of Glacial Cache values has been provided after the Australia
Post PAC API services changes on the 8th of April, 2013.
