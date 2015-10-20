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


Note: TaxRates
==============
This module does not import or modify the sales tax tables downloaded from
TaxRates.com in any way. The files are used exactly as supplied, after being
copied to the "taxrates" folder on the server.

The module makes no warranty to the accuracy of the data provided in
the tables supplied from TaxRates.com

Is is important that you download new tables on a regular basis!

http://taxrates.com
