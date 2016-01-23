Description
===========
This module provides a price history feature for Drupal Commerce products.

Requirements
============
- Drupal Commerce: 7.x-1.0 (http://drupal.org/project/commerce)
- Field extra widgets: 7.x-1.0-beta1 (http://drupal.org/project/field_extrawidgets)

Installation
============
- Install the module as usual
- Navigate to admin/commerce/products/types (Store -> Products -> Product types)
- On the "manage fields" page, add a Price History field and select the 'Hidden' widget
- On the field edit page, attach your newly created field to an existing Price
  field in order to track it's changes (this is a one time operation, the
  "Attached Product Price field" select box will be disabled after saving the form)

Configuration
=============
- On the "manage display" page, you can configure a few settings for the price history chart formatter

Credits
=======
Developed and maintained by Andrei Mateescu (amateescu) - http://drupal.org/user/729614
