PRODUCT SPECIFICATION
---------------------

DESCRIPTION
-----------
The Product Specification module provides a specification field type for CCK.
It stores product specifications of ecommerce websites built using Drupal
commerce / Ubercart modules or user can use this module to store specifications
in any drupal entity type. Module provides a simple, generic form/widget to
input product specification data. The form allows the user to add or remove
rows in specification field, then enter data via text/textarea field.
Since this is a CCK field, it is automatically revision capable, multi-value 
capable and has prebuilt integration with Views.

INSTALLATION
------------
- Copy product_specification directory to /sites/all/modules
- Enable product specification module at /admin/modules
- Add a specification field to an entity at /admin/structure
- Enable product specification bulk import module at /admin/modules

CONFIGURATION
-------------
- Go to admin/config/content/product_specification to set the separator for
  the CSV import.

USAGE
-----

Bulk Import:

  + Go to specification/import.
  + Select the "Entity Type", "Bundle" and "Field Name" to import bulk 
    specifications.
  + Download the template for the selected combination.
  + Template has four auto generated columns.

  1. "Product ID/SKU/Node ID/User ID/Taxonomy ID" etc. 
  2. "Delta" is the multi-value column where delta values are 0, 1, 2 etc.
  3. "Name" specification name.
  4. "Value" specification value.

  + Please find the below example of image provided on project page.

  Example:

    "Node Id", "Delta", "Name", "Value"
    1, 0, 'CONVENIENCE FEATURES', ''
    1, 0, 'Memory Backup', 'Yes'
    1, 0, 'Other Convenience Features', 'Turbo Drum, Temp Selection: Cold,
    Hot / Cold Water Inlet: Cold, 3 + Punch Pulsator'
    1, 0, 'Digital Display', 'No'
    1, 1, 'GENERAL', ''
    1, 1, 'Load Type', 'Top Loading'
    1, 1, 'Water Level Settings', '4'
    1, 1, 'Water Level Selector', 'Yes'
    1, 1, 'Shade', 'Cool Grey'
    1, 2, 'IN THE BOX', ''
    1, 2, '1 Washing Machine, 2 Inlet Hose, Drain Hose,
    Owner Manual, Span Ring, Anti Rat Cover', ''  

  + User can try and import this example to see how the product.
    specification bulk import works.
  + Keep Name and Value column blank for first row of every delta, If you don't
    want header/caption.
  + User can add html/php data in specification field.

AUTHOR/MAINTAINER
-----------------
- Mayur Jadhav.
-
