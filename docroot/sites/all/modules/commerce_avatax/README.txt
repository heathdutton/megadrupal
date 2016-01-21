INTRODUCTION
============
The Drupal Commerce Connector for AvaTax is a Drupal compliant module that
integrates the Drupal Commerce check-out process with AvaTax from Avalara, Inc. 
and is used for sales tax calculations and sales tax compliance.

AvaTax reduces the audit risk to a company with a cloud-based sales tax
services that makes it simple to do rate calculation while managing exemption
certificates, filing forms and remitting payments.

The module supports two modes - Basic - Pro - which correspond to
the levels of service available for AvaTax.

The AvaTax Basic service offers limited use of the AvaTax sales tax codes, in
particular P0000000 for Personal Taxable goods, Shipping tax codes and NT for
Not Taxable goods.

The AvaTax Pro service offers full use of the AvaTax sales tax codes. The Pro
service is required for states like New York were sales tax rates can be based
on the type of product, the delivery address, and the time of year.

Sales Tax is calculated based on the delivery address, the sales tax codes
assigned to line item in the order, and the sales tax rules applicable to the
states in which Nexus has been configured.

Access to a full development account can be requested by contacting
Avalara, Inc.

Version 4.3 of the module is compatible with Drupal 7.x and has been tested
with Drupal Commerce 1.x

There is an upgrade path from version 7.x-4.1 to version 7.x-4.3
i) Follow the Drupal procedure for back up and site maintenance mode.
ii) Update the module files
iii) Run "update.php" - and follow the instructions provided
iv) Take the site out of maintenance mode

There is no upgrade for version 7.x-4.1 Trial mode. Please disable and
completely un-install version 4.1 of the module. Install and enable
version 4.3

The Community Edition of the module, provided with version 4.2 has been
discontinued. Please disable and completely un-install version 4.1
Community Edition of the module. Install and enable version 4.3

REQUIREMENTS
============
a) The service uses the AvaTax ReST api for processing transactions.
b) The server PHP configuration must support cURL


NEW INSTALLATION
=================
Installing the module is done as for any custom Drupal Commerce module

a) Unzip & copy the folder "commerce_avatax" to the location shown below,
or in accordance with your Drupal Commerce configuration.

yoursite/sites/all/modules/commerce_avatax

b) Enable the two modules (AvaTax & AvaTax UI) in the usual way.

c) Do NOT enable the companion module - AvaTax Exemptions - unless you sell
goods to sales tax exempt organizations, and wish to allow new buyers to
purchase goods without being charged sales tax prior to registering their
sales tax exemption certificate.

Instructions for the AvaTax Exemptions sub-module are at bottom of this README


CONFIGURATION
=============
Select Store -> Configuration -> AvaTax settings

Complete the information requested, as is applicable to edition selected.
Save the form - AvaTax settings - on completion.

GENERAL
-------
Select AvaTax Version
---------------------
AvaTax Basic: Requires AvaTax account, and a configured company.
AvaTax Pro: Requires AvaTax account, a configured company, and AvaTax tax codes.

Sales tax description
---------------------
Enter the Sales tax description to be used on the order review page

AvaTax BASIC & AvaTax PRO
-------------------------
Shipping settings
-----------------
i) AvaTax States
Select which states will use AvaTax Basic sales tax service. Leave blank to
use AvaTax Basic for all states.

ii) Shipping Tax Code
FR020100 is the AvaTax sales tax code for shipping by public carrier
(USPS, Fedex, etc)- without any change to the rate charged.

iii) Destination Address to Use for Sales Tax
Select Shipping unless your site ONLY sells digital goods.
Sales Tax regulations state that the delivery address of digital goods is the
customers billing address. If your site sells both digital and physical goods,
please select Shipping.

If the site allows a purchase of physical and digital goods on one order,
please contact adTumbler to discuss how to extend the module.

iv) Business or Company Name:
Enter the registered business/company name

v) Primary Address:
NB - Sales tax law requires that a sales tax transaction record the place from
which good are shipped. This version of the module does not support Drop
Shipments. However, sales tax will be correctly calculated so long as a
valid primary office address is entered.

Address Validation
------------------
Configure as required - only available if Shipping Selected.

Note: The module will validate addresses in all USA states. This is a separate
from the single state of Nexus, where sales tax is calculated.

Credentials
-----------
i) Select Development or Production.
Only select production if you have a production account with AvaTax

ii) Company Code
Enter the AvaTax company code for the company you have configured (in case of
development was configured for you) in your AvaTax account. Please make sure
that you have configured Nexus for the company correctly.

iii) Enter the Production or Development Account number

iv) Enter the Production or Development License key

v) Use Validate credentials to test connectivity to AvaTax for the Community
Edition credentials received.

Sales Order Processing
----------------------
Select as required

We strongly suggest that this option is only selected after the module is
installed and working. There are significant sales tax compliance requirements
to automating the sales tax order processing and completion process.

This option is implemented by enabling 3 rules - these can be customized
according to the business requirements.

AvaTax Exemption Settings
-------------------------
If selected, the module will create a "sales_tax_exemption" field on the user
entity when the administration form is saved.

This field is created with a default value of " " - which means sales tax is 
to be calculated for this account.

To add an exemption type to a user:
Select the menu option - admin/people - edit user - select the
sales tax exemption code applicable - save the user entity

The exemption value will be passed to AvaTax with any transaction made by
this user when logged in to their user account. AvaTax will use this value
to override any regular sales tax for transaction, according to the sales tax
rules applicable to the state and address used for tax ability.

Note: A company is required to keep a valid copy of the sales tax exemption
certificate - for any user for whom sales tax is not charged!

AvaTax PRO ONLY
---------------
Configure Pro as above

Enabling the Pro version of the module will:
i) Create a sales tax vocabulary to manage the AvaTax Pro sales tax codes
to be assigned to each products.
ii) Create a sales tax code field for selected Product Types

The AvaTax Go Live team will work with you to determine what AvaTax
sales tax codes should be assigned to each product.

Add the required sales tax codes to your sales tax vocabulary.

Add the correct sales tax code to each product in your catalog. If no sales
tax code is assigned to a product, the module will default to tax code P0000000


OPERATION OF MODULE
===================
i) The sales tax transaction is created in the AvaTax cloud service at the same
time Drupal Commerce allocates an order number to the order object. This is
done in order to synchronize the Drupal Commerce order number with the
AvaTax transaction.

ii) A separate line item is added to the Drupal Commerce Order for the sales
tax amount.

iii) If creating orders using order administration, you are required to add the
product lines, the shipping lines, enter a valid address and then save the
order to allocate an order number. One this has been done, you can add a
sales tax line item to the order.

iv) If anything is updated on an order that can affect sales tax (address,
quantity or price) we suggest that you make the changes and delete the AvaTax
line item. Save the order to reflect the updates. Then add a new sales tax
line item to the order. AvaTax will recalculate the sales tax and write the
new values into the original  transaction. This assumes that the AvaTax
entry has not been committed (refer sales order processing).


MATCHING LINE ITEMS TO SALES TAX CODES - BASIC SERVICE
======================================================
The computation of Sales Tax Rates and the Sales Tax Amount is determined by
the configuration of the AvaTax account, in particular Nexus configuration.

All Product line items default to - P0000000 - Personal Taxable Goods.

The Discount coupon line item in the order defaults to the sales tax code
- OD010000 - discount on Personal Taxable Goods.

The Shipping product line item is defined by the sales tax administrator
- default - FR020100 - Public Carrier at prices quoted.

The AvaTax cloud service allows the creation of custom Tax Codes, Tax rules
and the assignment (Items) of Drupal Commerce product codes to a sales tax code.
Please refer to a sales tax advisor for sales tax compliance.

If you supply Taxable Products that are NOT Tangible Personal Property,
you are required to set up your own Tax Codes and Tax Rules for these products.
(See Pro service for use of AvaTax Tax Codes and Tax Rules)

The Item function in the dashboard is used to map each taxable Product Code
established in the Drupal Commerce catalogue (that is NOT Tangible personal
Property) to the Tax Code supporting it's tax rules and rates.

If you supply Products that are EXEMPT of sales tax in SOME states you
are required to set up your own Tax Codes and Tax Rules for these products.
See Pro service for use of AvaTax Tax Codes and Tax Rules)

The Item function in the dashboard is used to map each EXEMPT Product Code
established in the Drupal Commerce catalogue to the Tax Code supporting it's
tax rules and rates.

NB: If you supply Products that are EXEMPT of sales tax in all states where
you have Nexus (collect sales tax) you may use the Item function in the
dashboard to map each EXEMPT Product Code established in the Drupal Commerce
catalogue to the AvaTax Tax Code - NT - Non Taxable Goods


MATCHING LINE ITEMS TO SALES TAX CODES - PRO SERVICE
====================================================
Please contact Avalara, Inc for details on assigning sales tax codes
to product items


PRODUCTION ACCOUNT - GO LIVE PROCESS
====================================
The production account number and license key will be supplied to the
sales tax administrator after they have completed the Go Live training
call with Avalara.

Replace the Development Company Code provided with the development account,
with the company code created for the production company

Select Production - enter the new account information.

Save the form.


NB: Do check with the sales tax administrator that any custom tax codes,
and Nexus configurations, created in the development account have been
configured for the company created in the production account.

It is suggested that customers set up two companies in their production
AvaTax account - a test company for test transactions - and a production
company for normal operations.


UPGRADE FROM EARLIER VERSIONS
=============================
There is no upgrade path provided for 7.x-1.x or 7.x-2.x or 7.x-3.x to
version 7.x-4.x of the module - which uses the new REST api from Avalara.

Please contact - Avalara Support - for help on upgrading.

The AvaTax module stores data in the table called "variable"

7.x-1.x was implemented extending a hook from the Sales Tax module. It did not
create a sales tax line item. Sales tax data was saved as data elements in the
product line item. After removal sales tax amounts may not be correctly
displayed on old orders.

7.x-2.x implemented a custom sales tax line item based on the shipping module,
and a new checkout page. The requirement for a sales tax check-out page was
removed at the request of the community.

DO NOT attempt to install this module with an earlier version.

We apologize for the architectural changes between earlier versions and suggest
that Drupal databases are backed up prior to any upgrade.


AvaTax Exemptions
=================
Enable the modules AvaTax Exemptions in the usual Drupal way.

i) Select Store -> Configuration -> Checkout settings

ii) Select Sales tax -> configure - in the Checkout pane of the form

iii) Select the states for which the sales tax exemption pane can be displayed.
This is usually the states that the company has Nexus

iv) Select the user roles for which the sales tax exemption pane will be
displayed. This is usually for "anonymous user" only. Registered users will
either have their sales tax exemption certificate recorded in AvaTax - or
entered as a field in their Drupal User account

v) Enter the message that will be shown to the user in this pane. The message
should explain that only a buyer who has a valid exemption certificate may 
elect to not be charged sales tax, that wrongful use of this option is an 
offence, and what the policy is for obtaining a copy of the exemption
certificate is prior to goods being shipped.


Open Issues
===========
i) Refer Operation of Module - for correct way to manually create or update
an order.
ii) When using the Checkout Progress module.
If a user reviews an order, the selects the shipping page link to go back
(as opposed to selecting go back from the bottom of the check out page) there
is a status bug in Checkout Shipping, that prevents the sales tax line from
being recalculated if the shipping amount is changed.

Feature Requests
================
i) The module currently supports one shipping sales tax code. Please contact
Avalara if you have a mixture of "cost plus" shipping - and "at cost"
shipping - as these require different sales tax codes to be applied.
