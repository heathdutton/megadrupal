INTRODUCTION
============
The Ubercart Connector for Tax Rates is a Drupal compliant module that
integrates the Ubercart check-out process with sales tax calculations
based on sales tax by zip code data downloaded from the web site TaxRates.com.  
 
Sales Tax is calculated based on the zip code of either the order shipping
address (or the order billing address for digital goods) and the sales tax rate
applicable to that zip code.

Version 7.x-1.0 has been tested with Drupal 7.x and Ubercart 3.x 


REQUIREMENTS
============
The sales tax tables for each State in which taxes are to be calculated (Nexus)
need to be downloaded from the web site TaxRates.com & uploaded to the
folder called "taxrates" - a sub directory of the module directory! 


INSTALLATION
============
a) Unzip & copy the folder "uc_taxrates" to the location shown below,
or in accordance with your Ubercart configuration.

yoursite/sites/all/modules/ubercart/uc_taxrates/*.*

b) Enable the two modules (TaxRates & TaxRates UI) in the usual way. The
modules are shown under Ubercart - extra.

c) Copy files downloaded from TaxRates.com to the folder
yoursite/sites/all/modules/ubercart/uc_taxrates/taxtables/*.*

A separate file is downloaded from TaxRates.com for each state

The file name follows a convention e.g. TAXRATES_ZIP5_CA201312.csv

i) The fixed format - "TAXRATES_ZIP5_"
ii) The State of the billing address - derived from the checkout form
iii) The year - configured in settings
iv) The month - configured in settings

Please inspect the names of the TaxRates files downloaded - identify the year
and month of you files:

NB: If you have multiple files for multiple states they must all end
with the same date values.

If the module does not find a TaxRates file for the computed TaxRates.com
file - or locate a matching zip code in the file - it will report an error.


CONFIGURATION
=============
Select:
Store administration -> Configuration -> Ubercart Connector for TaxRate settings

Complete the information requested after reading the following notes.
Save the form - Ubercart Connector for TaxRates settings - on completion.

TaxRate tax table year
----------------------
Enter the year that matches the year element of the TaxRate file name

TaxRate tax table month
-----------------------
Enter the month that matches the month element of the TaxRate file name

TaxRates Selected states
------------------------
The module requires you to select each state for which it calculates sales tax.

Note: Each state will require it's own csv file. All states MUST have the same
month and year in the file names!

It is important that you consult with an accountant or sales tax advisor on 
your legal obligations to collect Sales Tax - known as Sales Tax Nexus.

Select Destination Address to use for Sales Tax
-----------------------------------------------
Select Shipping unless your site ONLY sells digital goods. Sales Tax regulations
require that the taxable address for digital goods is the customers billing
address.

Note: Sales tax is calculated on the shipping line item of an order for the 
following states - assuming that the amount charged for shipping is by public
carrier - and that this has been charged directly to the customer.

Arkansas, Connecticut, District of Columbia, Georgia, Hawaii, Indiana, Kansas,
Kentucky, Michigan, Minnesota, Mississippi, Nebraska, New Jersey, New Mexico,
New York, North Carolina, North Dakota, Ohio, Pennsylvania, South Carolina,
South Dakota, Tennessee, Texas, Vermont, Washington, West Virginia, Wyoming

For additional information of shipping and sales tax we suggest reviewing the
following blog - contacting a sales tax advisor - or http://avalara.com

http://blog.taxjar.com/sales-tax-and-shipping/

Sales Tax Description
---------------------
The sales tax description to be shown to the user. 

Show location code
------------------
If selected the sales tax description will include the delivery city entered
by the user in the shipping/billing address field at checkout.  

Show zero taxes
---------------
Will display zero when sales tax is correctly calculated to be zero by the 
module using a TaxRate table. The module will return no value for states
not selected. The module can be used at the same time as the Ubercart Sales
Tax module.

OPERATION OF MODULE
==================
Sales Tax is calculated and displayed on completion of the delivery/billing
information during the checkout process.

If the module does not find a rate file from TaxRates.com - after computing the
required file name of the format described above (static, state, date.csv)
in the "taxrate" module directory on the server it will ERROR.

Please do check that you have added TaxRates.com files to the module directory
for all the states selected.

Please do update the sales tax tables regularly. Download the latest file for
each state. Copy it/them the "taxrates" directory on the server, and
configure the module to use the new year and month.

Please be aware that sales tax boundaries do not map exactly onto sales tax 
districts. Zip Codes are only an approximation of sales tax jurisdictions.


Note 2: Sales Tax Rates
=======================
This module does not import or modify the sales tax tables downloaded from
TaxRates.com in any way. The files are used exactly as supplied, after being
copied to the "taxrates" folder on the server.

The module makes no warranty to the accuracy of the data provided in
the tables supplied from TaxRates.com


Open Issues
===========
None reported

Please eMail Alex - bischoff.alex@adtumbler.com - if you would like to test the
cloud computing service from Avalara, Inc. - called AvaTax.
