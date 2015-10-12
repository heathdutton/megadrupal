This module provides a reporting system for Drupal Commerce.

Some features that you will find in this module:

* Table-based reports for sales with filtering by order status and date ranges.
  The report can break sales down monthly, weekly and daily.
* Table-based reports for customers, products and payment methods.
* An advanced reports dashboard showing a summarized view for all the above
  reports, visualization done by Charts module, using Google Charts.
* The ability to export to CSV files when Views Data Export is enabled.

Sub-modules
=============================

Commerce Reports Tax
-----------------------------
The tax reporting module uses the Batch API to generate reports. It is able to
handle multiple reports with a variety of parameters, which are handled through
the Entity API.

Commerce Reports Geckoboard
-----------------------------
This sub-module provides integration with Geckoboard allowing for reporting
information about your revenue, sales and orders.

Commerce Reports Stock
-----------------------------
The stock calculates stock reports.


Notes
===========================

1. Upgrading from 7.x-3.x to 7.x-4.x
---------------------------
Since the 4.x branch has migrated from Visualization API to Charts, it is
recommended that Commerce Reports 3.x is disabled and uninstalled. It is also
recommended that you check your active views (admin/structure/views) and ensure
all of Commerce Reports' views have been removed.

2. Order created dates
---------------------------
Commerce Reports is based off of an order's CREATED date. Some modules, such as
Commerce Google Analytics trigger on an order's checkout completion. To provide
a better reporting environment it is recommended to enable the "Set the order
created date to the checkout completion date" rule. This will change an order's
created date to the check out time and synchronize Commerce Reports with
Commerce Google Analytics.

This rule was committed to Commerce dev November 4, 2013 by
Ryan Szrama https://drupal.org/node/2044231.

3. Product reports line item types
---------------------------
The product report is filtered to just the Product line item type. If your
Commerce site utilizes custom line items they will have to be enabled through
Views UI. This is default so that shipping line items and possible feed line
items are not accounted for.

4. Access Commerce Reports when using Commerce Kickstart
---------------------------
Commerce Kickstart uses a custom administrative menu which can make accessing
Commerce Reports difficult. The path to the reporting dashboard can be found at
/admin/commerce/reports.

5. Excluding fees or other price components (#2284209)
---------------------------
There is there Commerce price by components module which provides a field formatter
that will exclude price components from being included in a rendered price.

https://www.drupal.org/project/commerce_price_components

Changes From 7.x-3.x to 7.x-4.x
===========================

Dependencies
---------------------------
Removed:
  * Visualization API (visualization)

Added
  * Charts (charts, charts_google)
  * Date Popup (date_popup)
  * Views Date Format SQL (views_date_format_sql)
