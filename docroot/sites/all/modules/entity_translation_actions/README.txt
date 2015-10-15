
INTRODUCTION
------------

Module provides actions for management translations of entities using Views Bulk
Operations module.

* For a full description of the module, visit the project page:
  http://drupal.org/project/entity_translation_actions
* To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/entity_translation_actions


REQUIREMENTS
------------

This module requires the following modules:
 * Entity translations (https://drupal.org/project/entity_translation);
 * Views Bulk Operations (https://drupal.org/project/views_bulk_operations).


INSTALLATION
------------

Install as usual, see
https://drupal.org/documentation/install/modules-themes/modules-7 
for further information.


CONFIGURATION
-------------

Create new view for entities list in Administration » Structure » Views
» Add ( /admin/structure/views/add ).

  - Select type of entity. Click "Continue & edit".
  - For a new view add field "Bulk operations".
  - In group "Selected bulk operations" enable option "Modify entity
    translations".
  - Add other fields and filters for view.


USAGE
-----

Got to page of created view. Select list of entities and apply operation
"Modify entity translations". On next page select operation from a list.

* Add translation. This operation will copy existed translation and create new
  translation for selected language.

* Set translation. This operation will update entity language. 
  If translation for selected language doesn`t exist - it will be created based 
  on old entity language.

* Delete translation. This operation will delete translation for selected
  language.
  If selected language equal to entity language - entity language will be  set
  to "und".


CONTACT
-------

Current maintainers:
* Adyax (Open Source Experts) - https://drupal.org/marketplace/adyax
* Vitalii Tkachenko (vtkachenko) - https://drupal.org/user/2921049
