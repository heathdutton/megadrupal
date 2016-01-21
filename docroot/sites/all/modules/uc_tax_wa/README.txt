
This is a module for calculating Washington State Sales tax in
Ubercart, in Drupal. 


INSTALLATION AND SETUP

a) Install and enable the module in the normal way for Drupal.

b) Visit your Ubercart Store Administration page, Configuration
section, and choose "Washington state sales tax settings". (path:
admin/store/settings/uc_tax_wa)

c) On this page, settings: 
   - Default sales tax rate and location code, in case address lookup fails
   - Whether to show the location code in the order
   - Product types to be taxed
   - Line items to be taxed (such as shipping charges, see
      http://apps.leg.wa.gov/wac/default.aspx?cite=458-20-110)

Your location code may be found using the tool at
http://dor.wa.gov/content/findtaxesandrates/salesandusetaxrates/lookupataxrate/

    
OPERATION OF MODULE

Once you install and configure the module, if someone makes an order
and the Delivery Address is a Washington address, all taxable products
and line items that you configured will be taxed according to the
customer's tax rate. If the order has no shippable items, Ubercart
will not collect a delivery address, so the Billing Address is used.

The rate is determined from the Washington Department of Revenue's
lookup service.  (See
http://dor.wa.gov/Content/FindTaxesAndRates/RetailSalesTax/DestinationBased/ClientInterface.aspx
for more information about the tax lookup service.) If the rate lookup
fails, the default tax rate you configured will be used instead.

Once the rate is found, the tax is calculated by multiplying the
product quantity * price * rate for each taxable product in the order,
plus line item price * rate for each taxable line item, and the
Ubercart Tax module handles actually adding the tax line item to the
order.

The initial versions of this module were released at 
http://www.ubercart.org/contrib/8311.

This module was developed by Jennifer Hodgdon of Poplar ProductivityWare.
http://poplarware.com
