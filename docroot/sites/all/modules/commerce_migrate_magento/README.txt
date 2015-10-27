Commerce Migrate Magento README
--------------------------------

This module is way to import Magento site into Drupal
Commerce. It takes into account many of the standard features of Magento
and of Drupal Commerce, but since real installations can vary quite widely,
it would be unreasonable to expect this stock migration technique to bring all
of your data over from an arbitrary Magento site.


Assumptions and Limitations:
----------------------------
* Currently, the line item and order do not understand their dependency on
  products. You *must* import products of all product types before importing
  line items or orders. And if rolling back and starting over, you must make
  sure there are no existing line items or orders in the database before
  importing products. And make sure there are no products before importing
  products as well, because existing line items or product reference fields
  that have references to products will cause product deletion to fail.
* You need to choose whether you're going to assign UIDs ownership on incoming
  products and  product display nodes. If you want to do so, then the users need 
  to exist in the system.
* Visit /admin/content/migrate/magento_migration_options to set the options
  for your import. You need to tell it where the source database and files are.
* If you want the URL aliases (paths) set on Magento product nodes to become
  paths on the resulting product display nodes, enable the path module. If you
  do not want to bring paths over, then disable path module.
* The product image field(s) are configurable; the destination fields will be
  named the same as the source fields.
