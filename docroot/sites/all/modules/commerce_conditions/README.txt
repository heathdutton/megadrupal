The module adds additional Rules conditions related to Drupal Commerce. The 
conditions require a Taxonomy reference field to be present on the product (not
the product display) you are trying to check against.

It currently adds:

A condition for checking if a line item references a product with a certain
Taxonomy term applied to it:
Create a Rule, add the new condition called "Line item product has term"

A condition for comparing the quantity of products in a cart with a certain
Taxonomy term applied to them against a specified value:
Create a Rule, add the new condition called "Term-based product quantity
comparison"
