Commerce Migrate Ubercart README
--------------------------------

This module is way to import a D6 or D7 Ubercart site into Drupal
Commerce. It takes into account many of the standard features of Ubercart
and of Drupal Commerce, but since real installations can vary quite widely,
it would be unreasonable to expect this stock migration technique to bring all
of your data over from an arbitrary Ubercart site.


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
  products, product display nodes, orders, and customer profiles. If you want
  to do so, then the users need to exist in the system (This would normally
  be a self-upgrade from the ubercart site, or an upgraded site that we're
  now pulling the store into.
* Visit /admin/content/migrate/ubercart_migration_options to set the options
  for your import. You need to tell it where the source database and files are.
* If you want the URL aliases (paths) set on Ubercart product nodes to become
  paths on the resulting product display nodes, enable the path module. If you
  do not want to bring paths over, then disable path module.
* The product image field(s) are configurable; the destination fields will be
  named the same as the source fields.
* You must have a product_type for each type that was defined in the old site,
  the module will prompt you to do so if it has not been done.
* You must have an existing Product Display content type named product_display.
* See http://drupal.org/node/1206776#comment-4685032 for one import recipe.
