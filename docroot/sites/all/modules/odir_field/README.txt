Module page: http://drupal.org/sandbox/luxpaparazzi/1301730

This project defines a field type, allowing to integrate entities
with "Directory based organisational layer"

The field display shows the odir image files (jpegs), which are clickable.

An optional block shows associated nodes.


Installation steps:
* Enable the module "Field to directory"
  Optionally enable the module "Field to directory (block)"

* Go to admin/structure/types and create an odir field

* Create a field of type "Directory" and 
  associate it to one or more content types.
  (NOTE: In theory you may add as many directory fields as you want, 
  but the module is only tested with one filed)

* Optionally enable the following blocks: 
    - "Nodes assossicated to directory ([field_name])"
    - "Subdirectories ([field_name])"
    - "Directory operations ([field_name])
  (requires the "Field to directory (block)" module)"

* Enable the new blocks. Blocks from the odir module are dublicated.
  You may experiment in how to use the blocks from the main module
  or the field-specific module.

* Open and safe the display section of a content type, for 
  correctly enabling the display.

* Create and edit content and put the directoy path
  into the new directory field.

What you can do from here:

* While viewing content with a directory field attached,
  you will see a slideshow with images found in the directory

* If you enabled the block from "Directory Field (block)", you see a block
  with all the associated nodes.
