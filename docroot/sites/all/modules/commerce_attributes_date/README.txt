Commerce Attributes Date
------------------------

Description
-----------

This module provides the ability to use date field type as Product Attributes.
Currently only support date field type.

NOTE: If you're using date field with start and end date,
you need to apply a patch provided with this module,
named as "commerce_cart-date_field_with_end_date.patch"


Permissions
-----------

There are no extra permissions required for this module to work.


Installation
------------

To install, place the entire commerce_attributes_date folder into
your site's module directory.
From the Drupal administration section, go to Administer => Modules
and enable the Commerce Attributes Date module and its dependencies.

Updates
-------

N/A


Configurations
--------------

Before you start using this module,
you will need to configure the field type you want to allow to be used as 
product attributes.
To select field types to be used as attributes, follow below steps:
1. Go to Administer -> Store Settings -> Commerce Attributes Date
2. Select field types in "Allow attributes to"
3. Enter the product reference field machine name in
   "Product Reference Field Name".
4. Select date format to use "Date format" for showing dates.
   NOTE: This format will be used while displaying the date on product page
5. Click on Save Configuration.

Once you've configured allowed field types,
navigate to manage field section in your product variation.
Add field of field type you've configured in Commerce Attributes Date settings
In field configuration page,
navigate to "Attribute field settings" section and
configure field to be used as attributes by selecting
"Enable this field to function as an attribute field on Add to Cart forms"
checkbox.


Known incompatibilities
-----------------------

1. The module only works with one product reference field.

Issues
------

If you have any concerns or found any issue with this module
please don't hesitate to add them in issue queue.


Maintainers
-----------
The Commerce Attributes Date was originally developed by:
Yogesh S. Chaugule

Current maintainers:
Yogesh S. Chaugule
