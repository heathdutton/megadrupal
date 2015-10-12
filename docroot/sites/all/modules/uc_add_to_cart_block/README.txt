The UC_ADD_TO_CART_BLOCK module permits the "add to cart" ubercart product 
form to appear within a customizable block. By default, it creates a one
block called "Add to Cart" in your system page. This block is accessible 
to Panels, and Context, it is not cached, and is only visible on nodes 
which are of a known product class.

To theme this block, create a block template with the filename:
'block--uc-add-to-cart-block.tpl.php'. At minimum it must contain a 
'subject' and 'content' variables. Without the content variable, your add-
to-cart form will not display.

This module also provides a "Buy it now" block. For certain use cases, 
attribute enforcement gets in the way of adding a product to the cart from 
this method. This block has a setting that permits administrators to 
disable "attribute validation" for the "buy it now" form. 