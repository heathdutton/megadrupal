Description
===========
Ubercart Product Actions provides a set of Drupal Actions for usage by Ubercart
shop administrators for conducting site-wide changes to product data, using
custom codes or the excellent Views Bulk Operations (VBO) module.

It does so by providing a few new actions:

- Modify product Weight
- Modify product Sell Price
- Modify product List Price
- Modify product Cost Price
- Modify Default Quantity to add to cart

These actions can be used to manipulate these values on multiple products, using
three methods:

1. Percentage - alter values up or down by a percentage.
   (for example: increase the cost of selected products by 10%.)
2. Difference - alter values up or down by a fixed amount.
   (for example: decrease the price of all products by $5.)
3. Absolute - set values to a specified number.
   (for example: set the price on selected items to $15.)

Usage
=====
The module does nothing on its own. Here's how to quickly get it up and running:
 1) Download and install the Views and VBO modules:
    http://drupal.org/project/views
	  http://drupal.org/project/vbo
 2) Create a new node view, using a "Page" display and "Bulk Operations" as the display style.
 3) Filter the view to display products only, and expose some filters so that
    you can filter the list of products as you wish.
 4) In the VBO settings of that view, check the boxes next to the actions you wish to enable.
 5) Save the view, and visit the page.
 6) Filter the list (or simply select some products) and select the desired
    action from the Bulk Operations select box, and click Execute.
 7) On this screen enter your desired change, and click "Next".
 8) Make sure you got the right products, and click "Confirm".

 You're done! ;)


If you have any problems/questions/ideas - please visit the Issue Queue and let us know:
http://drupal.org/project/issues/uc_product_actions

Author
======
Asaph (asak) - asaph at cpo.co.il