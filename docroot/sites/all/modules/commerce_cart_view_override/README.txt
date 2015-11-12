Commerce Cart View Override is packaged for download at http://drupal.org/project/commerce_cart_view_override.

How to use
[1] Create a new cart view
  - Clone the existing view "Shopping cart form", machine name "commerce_cart_form"
  - Save the new view

[2] Set the cart view to the new view
  - Navigate to Administer >> Store >> Configuration >> Cart Overrides
  - Select the new view
  - Save Configuration

[3] If the new cart view displays fields from the product entity, then the
    the following permissions will need updated so that the roles that
    "can checkout" can see the cart view:
    "View any product of any type",
    "View any Product product", or
    "View any {YOUR SPECIFIC PRODUCT TYPE} product"
