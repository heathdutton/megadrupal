Views Sort Null Field
=====================

This module has for motivation the following problem:

  - You have a view listing nodes.
  - Yhese nodes have a field on them, 'field_foo'.
  - You want to sort the view by this field, ascending.
  - Some nodes have nothing in this field.
  - Because of the way SQL sorting works, the nodes with no value are at the top of the list.

Use this in combination with the regular sort to place empty values at the bottom of a view, rather than at the top.

Example
-------

Order a list of nodes by an integer weight field, with empty values at the bottom.

Add the following sorts to the view:

1. Weight field, null sort, ascending
2. Weight field, ascending
