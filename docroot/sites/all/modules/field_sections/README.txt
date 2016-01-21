CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

The Field Sections module provides field composed of textfield and textarea.
Moreover, the textarea field supports text format.


REQUIREMENTS
------------
This module does not have the required modules.


RECOMMENDED MODULES
-------------------
   
 * Field collection (https://www.drupal.org/project/field_collection):
   Provides a field-collection field, to which any number of fields can be
   attached.
   
 * ECK (https://www.drupal.org/project/eck) and 
   Inline Entity Form (https://www.drupal.org/project/inline_entity_form):
   Stores field values as a reference to a sub-entity which contains the actual
   sub-field values.
     
 * Multifield (https://www.drupal.org/project/multifield):
   Provides a true compound field solution.
   
 * Double field (https://www.drupal.org/project/double_field):
   Can split your fields up into two separate parts. Its functionality similar
   to Field Sections. However, it does not support text format for a textarea.
   

INSTALLATION
------------
Install as you would normally install a contributed Drupal module.
See: https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------
Once you enable this module you will have a new "Field Type" called "Sections"
at the Field UI (e.g. admin/structure/types/manage/CONTENT_TYPE/fields).
In order to disable the module you would have to delete the fields of this type
first.


MAINTAINERS
-----------
Current maintainers:
 * Pavel Shevchuk (pshevchuk) - https://www.drupal.org/u/pshevchuk
