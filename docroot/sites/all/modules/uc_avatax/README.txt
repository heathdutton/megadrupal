INTRODUCTION
============
The Ubercart Connector for AvaTax is a Drupal compliant module that
integrates the Ubercart check-out process with AvaTax from Avalara, Inc. 
and is used for sales tax calculations and sales tax compliance.

AvaTax reduces the audit risk to a company with a cloud-based sales tax
services that makes it simple to do rate calculation while managing exemption
certificates, filing forms and remitting payments.

The module supports three modes - Trial - Basic - Pro - which correspond to
the levels of service available for AvaTax.

The Trial configuration provide on-line registration for calculating the
sales tax amount in any state. The trial account provides no access to an
AvaTax account or user dashboard. Details of the sales tax transactions are
recorded in the Drupal logs.

Access to a development account can be requested by contacting Avalara, Inc.
or Alexander Bischoff at bischoff.alex@adtumbler.com

The AvaTax Basic service offers limited use of the AvaTax sales tax codes, in
particular P0000000 for Personal Taxable goods, Shipping tax codes and NT for
Not Taxable goods.

The AvaTax Pro service offers full use of the AvaTax sales tax codes. The Pro
service is required for states like New York were sales tax rates can be based
on the type of product, the delivery address, and the time of year.

Sales Tax is calculated based on the delivery address, the sales tax codes
assigned to line item in the order, and the sales tax rules applicable to the
states in which Nexus has been set up.

Version 4.x of the module is compatible with Drupal 7.x and has been tested
with Ubercart 3.x

Access to the AvaTax service is provided at:

AvaTax Development service: https://admin-development.avalara.net
AvaTax Production service: https://admin-avatax.avalara.net 

REQUIREMENTS
============
a) The service uses the AvaTax REST api for processing transactions.
b) The server PHP configuration must support cURL

The module install file will check for cURL and report the result to the
Administrator Status report. Administrators are requested to check the Status
report after enabling the module to ensure cURL shows as enabled for AvaTax.


NEW INSTALLATION
=================
Installing the module is done as for any custom Ubercart module

a) Unzip & copy the folder "uc_avatax" to the location shown below,
or in accordance with your Ubercart configuration.

yoursite/sites/all/modules/ubercart/uc_avatax/*.*

b) Enable the two modules (AvaTax Calc & AvaTax UI) in the usual way. The
modules are shown under Ubercart - extra.


CONFIGURATION
=============
a) Select Store -> Configuration -> AvaTax settings

Complete the information requested after reading the following notes.
Save the form - AvaTax settings - on completion.

General settings
================
AvaTax Version
--------------
Trial - Enter your name and email address. Fetch Trial credentials.
Basic or Pro - Only if you have an AvaTax account and have set up a company.

Company Code
------------
Enter the AvaTax company code for the company you have configured (in case of
development was configured for you) in your AvaTax account. Please make sure
that you have configured Nexus for the company correctly.

Sales tax description
---------------------
Enter the Sales tax description to be used on the order review page

Show location code
------------------
If selected the sales tax description will include the delivery city entered
by the user in the shipping/billing address field at checkout.  

Show zero taxes
---------------
Will display zero when sales tax is calculated to be zero by the AvaTax
service. If configuring the module to use a selection of states, the module 
will make no return for orders not using the AvaTax service. The module can
be used at the same time as the Ubercart Sales Tax module.

Shipping settings
=================
Avatax Selected states
----------------------
The module allows limiting  sales tax calculations using the AvaTax service
to explicitly selected states. The default option is for this selection to be
left blank. All orders will be sent to the AvaTax service on checkout.

It is important that you consult with an accountant or sales tax advisor on
your legal obligations to collect Sales Tax.

Select "--" to use module for address validation only

Shipping Tax Code
-----------------
FR020100 is the AvaTax sales tax code when shipping by public carrier
(USPS, Fedex, etc.)- without change to their rates - please refer
to Avalara for the correct sales tax codes codes if you do not use public
carriers or bill additional charges for shipping. The module only caters for
one category of shipping. The Pro version of the module can be extended to
support these requirements.   

Select destination Address to use for Sales Tax
-----------------------------------------------
Select Shipping unless your site ONLY sells digital goods.
Sales Tax regulations state that the delivery address of digital goods is the
customers billing address. If your site sells both digital and physical goods,
please select Shipping.

If the site allows a combination of physical and digital goods on one order
please contact adTumbler to discuss how to extend the module.

Primary Address
---------------
NB - Sales tax law requires that a sales tax transaction record the place from
which good are shipped. This version of the module does not support Drop
Shipments. However, sales tax will be correctly calculated so long as a
valid primary office address is entered.

Credentials
===========
Only applicable when Basic or Pro versions selected

AvaTax Mode
-----------
Only select Production if you have completed the GO LIVE process with Avalara
and have a valid production account and license key

AvaTax Account Number
---------------------
Enter the AvaTax Account number provided.

AvaTax License Key
------------------
Enter the AvaTax License Key for your account

Use Validate credentials to test connectivity to AvaTax for the
account # and license key entered

Sales Order Processing
======================
Configure as required - only available if Basic - Pro - selected

We strongly suggest that this option is only selected after the module is
installed and working. There are significant sales tax compliance requirements
to automating the sales tax order processing and completion process.

This option is implemented by enabling 3 rules - these can be customized
according to the business requirements.

AvaTax Exemption Settings
=========================
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


Configure AvaTax Pro
====================
i) Create a new PRODUCT taxonomy called "AvaTax Tax codes"
ii) Add the required AvaTax Sales Tax codes as taxonomy items. At a minimum
add the three items - NT - P0000000 - PB100000

iii) Select - yoursite/admin/structure/types/manage/product/fields

Add a new field as follows:

Label: avatax_code
Machine Name: field_avatax_code
Field Type: Term Reference
Widget: Autocomplete Term widget

The AvaTax Go Live team will work with you to determine what AvaTax
sales tax codes should be assigned to each product.

Add the correct sales tax code to each product in your catalogue. If no sales
tax code is assigned to a product, the module will default to tax code P0000000


OPERATION OF MODULE
===================
a) To test connectivity to AvaTax. Process an order to a valid delivery address.
Visually inspect the cart for inclusion of a sales tax amount in the review
order form.

An AvaTax error will (caused by an incorrectly configured account/license key)
display an error clearly on the check out form.

b) The sales tax transaction is created in the AvaTax cloud service at the same
time Ubercart allocates an order number to the order object. This is done in
order to synchronize the Ubercart order number with the AvaTax transaction.

c) The module will support sales tax for orders entered manually using order
administration. 

e) If anything is updated on an order that can affect sales tax (address,
quantity or price) make the changes and update.

NB: Do not update an order AFTER it has been committed in AvaTax


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
and the assignment (Items) of Ubercart product codes to a sales tax code.
Please refer to a sales tax advisor for sales tax compliance.

If you supply Taxable Products that are NOT Tangible Personal Property,
you are required to set up your own Tax Codes and Tax Rules for these products.
(See Pro service for use of AvaTax Tax Codes and Tax Rules)

The Item function in the dashboard is used to map each taxable Product Code
established in the Ubercart catalogue (that is NOT Tangible personal
Property) to the Tax Code supporting it's tax rules and rates.

If you supply Products that are EXEMPT of sales tax in SOME states you
are required to set up your own Tax Codes and Tax Rules for these products.
See Pro service for use of AvaTax Tax Codes and Tax Rules)

The Item function in the dashboard is used to map each EXEMPT Product Code
established in the Ubercart catalogue to the Tax Code supporting it's
tax rules and rates.

NB: If you supply Products that are EXEMPT of sales tax in all states where
you have Nexus (collect sales tax) you may use the Item function in the
dashboard to map each EXEMPT Product Code established in the Ubercart
catalogue to the AvaTax Tax Code - NT - Non Taxable Goods.


MATCHING LINE ITEMS TO SALES TAX CODES - PRO SERVICE
====================================================
Please contact Avalara, Inc for details on assigning sales tax codes
to product items.


PRODUCTION ACCOUNT - GO LIVE PROCESS
====================================
The production account number and license key will be supplied to the
sales tax administrator after they have completed the Go Live training
call with Avalara.

Enter the Company Code created during the Go Live training

Select Production, and enter the new account and license key

Save the form.


NB: Do check with the sales tax administrator that any custom tax codes,
and Nexus configurations, created in the development account have been
configured for the company created in the production account.

It is suggested that customers set up two companies in their production
AvaTax account - a test company for test transactions - and a production
company for normal operations.


UPGRADE FROM EARLIER VERSIONS
=============================
DO NOT attempt to install this module with an earlier version.

There is no upgrade path provided for version 1 or 2 to this version
of the module - using the new REST api from Avalara.

Please contact - bischoff.alex@adtumber.com - for discussion on upgrading.

The AvaTax module stores data in the table called "variable" - please
fully un-install any earlier version of the module prior to installing
this module.

Do make a note of you company code - refer module configuration
Do make a note of the accounts and license key previously stored in the file
credentials.php 

We apologize for the major architectural changes between versions and suggest
that Drupal databases are backed up prior to an earlier version being removed.


Open Issues
===========
None reported
