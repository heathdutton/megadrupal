DESCRIPTION
-----------

Commerce Add to Cart radio provides a Views field handler that allow you to
build a `Commerce <http://drupal.org/project/commerce>`_ products view form
(using `Views <http://drupal.org/project/views>`_ module) but instead of display
multiples "Add to cart" buttons as the default Commerce field handler "Add to
Cart form" this handler displays as a radio input for each product with a global
form button.

This module is useful when you want to limit the user to chose a single product
that could be added to the cart from a list of offered products view. The best
example I can refer is sale of membership where the user must chose only one
membership type.

HOW IT WORKS
------------

Considering that Drupal Commerce provides great flexibility to easily build
forms through the use of Views this module motivate the use of this approach to
achieve custom forms that needs a UI where the user must add to cart a single
product. For that purpose this module provide a views field handler that takes
advantage of the Views form API to build a display of products with an input
radio element form.

Moreover, a quantity selection widget as input text or input select could be
configured so the user could chose a product and the quantity that want to add
to the cart from a list of products. Also is compatible with the line items
options that could be added to the form UI so the user can chose extra options
that must be added to the line item properties.

INSTALLATION
------------

- Enable this module as usual.

- Go to admin/structure/views and create a new view that shows Commerce Products
  and that display as Fields. Add all the product fields that you want in your
  view and the field provided by this module: "Add to cart (radio input form)"
  field.

- You can display an input text or input select element for quantity if the
  option "Display a quantity widget on the add to cart form" is enabled. The
  widgets are: "text or select". The first allow the user a free input of
  quantity, the second uses the range specified in "Range of quantity options"
  setting to build a defined range of quantity options for the input select.

- In case you use the quantity input select widget you can chose if display the
  price for each quantity option if the option "Quantity select total price" is
  enabled.

KNOWN ISSUES
------------

- We never tried the use of this module with products attributes and products
variations so to use in that conditions probably you will need to extend the
code of the field handler.

CREDITS
-------

- The development of this module was sponsored by `BlueSpark <http://www.bluespark.com>`_
