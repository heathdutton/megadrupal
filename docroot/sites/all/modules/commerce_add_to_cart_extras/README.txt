This module provides an alternative way of adding products to cart.


Overview
--------
The initial version provides a views field handler ("Commerce Product: Quantity
input field") that relies on the Views Form API to output a quantity textfield
that turns the view into an add to cart form.


Usage & Configuration
---------------------
To use this module, you'll need to make sure you use the Commerce Product as a
base for your view. Just add the field noted above and you are all set.

The default quantity can be configured, and only products with a quantity
larger than 0 are added to the cart.

By default, when a product is added to the cart from the form, it will link back
to the page which added the product originally, no matter if it was a view or a
product page. You can optionally configure it to go back to the first
referencing product it finds (which is fine if you only have a single product
display node) by selecting "Referencing Product" from the path option dropdown.
You can also select "Other..." and choose a custom path to link your product to.
