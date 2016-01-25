This simple Feature[1] for Drupal Commerce[2] provides an 'On sale' checkbox and
'Sale price' textfield that can be used on a per-product basis to override the
normal price of the product. It comes with a rule for applying the discount
which is enabled by default. It uses a discount price component type by default,
which makes it compatible with price formatting modules such as Commerce Price
Savings Formatter[3].

Note regarding Views
********************

The field 'Commerce Product: Price' in Views does not reflect any price
calculation rules, including this one. Commerce provides dynamic price
pre-calculation[4] but it's still a work in progress. You can enable Views
integration with this patch[5] from issue #1020050[6]. Although this opens up
the precalculated price data, it doesn't provide a method for updating an
individual product's precalculated price. I've put together a sandbox module[7]
that provides a rule that updates a product's precalculated price after it's
saved (you can change the event(s) it responds to). Please note the warning on
its project page - use at your own risk! Finally, only Commerce Sale Price
1.x-beta3 and later work correctly with Commerce's precalculated price system.

Upgrading from beta2
********************

Sorry, I wasn't careful enough naming the fields and rules to begin with, and to
avoid future clashes I decided to rename them for beta3. I haven't provided an
upgrade path as it's still beta. The following steps should be sufficient for
upgrading from beta2 to later.

Warning: this will destroy all the sale price data. 

1. Always do a DB backup before performing a module upgrade.
2. Install the new module files.
3. Revert the feature.
4. Clear all caches.
5. The new fields should now be attached to the product bundle; if you'd like
   them on other types then add as normal.
6. Delete the fields 'field_product_sale_price' and 'field_product_on_sale' from
   all product types they're attached to.

If you need to keep existing data, then you can create a View that lists
products with their two sale price fields and use the output of that to
manually update your products. If you have *lots of products* already with sale
price data, and you're not sure how to migrate the data from the old fields to
the new, please open a new support request. Apologies for the inconvenience.

Installation & usage
********************

1. Install as usual[8].
2. If you have a product type called product then the two fields will be added
   automatically to that type. (They can be removed if desired.)
3. To add the fields to other product types go to Store -> Products -> Product
   types -> [product type name] -> Manage fields.
4. Select existing field 'Boolean: field_commerce_saleprice_on_sale (On sale)',
   give it a label, choose Single on/off checkbox as the widget and click Save.
5. On the next page complete the help text and default value fields.
6. Click on the Manage display tab.
7. Ensure that both fields are configured to be hidden.
8. Repeat the last three steps with 'Price: field_commerce_saleprice (Sale
   price)' and the 'Price textfield' widget.

Sponsor
*******

This module is written by Pedalo, London based Drupal specialist web developers.
http://www.pedalo.co.uk/

Links
*****

[1] http://drupal.org/project/features
[2] http://drupal.org/project/commerce
[3] http://drupal.org/project/commerce_price_savings_formatter
[4] http://www.drupalcommerce.org/node/267
[5] http://drupal.org/files/commerce-views_precalculated_price-1020050-5.patch
[6] http://drupal.org/node/1020050
[7] http://drupal.org/sandbox/andy/1686666
[8] http://drupal.org/documentation/install/modules-themes/modules-7

author: AndyF
http://drupal.org/user/220112
