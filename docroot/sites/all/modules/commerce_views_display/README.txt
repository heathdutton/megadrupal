Commerce Views Display
====================

This module provides a views style plugin to render an add to cart form with
the view results.

Install instructions
====================
- Enable Commerce Views Display and dependencies

Creating a View:
- Create a view with the base table of Commerce Product
- Change the view's format to Commerce Add to Cart
- Alter the style options default settings as needed
- Recommended:
  -- Enable "Override pager and render add to cart form with all items." This
     will disable any views pager.
  -- Add the product commerce_price field to the display and ensure that
     a formatter with "Display the calculated sell price for the current user."
     is used.
  -- Add any other field that you would want to display for a selected product.


WARNING: The add to cart form views format will render the form for all products
in the views results.  This will cause every product to be loaded, so use with
caution for a large number of products.
