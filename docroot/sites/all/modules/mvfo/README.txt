INTRODUCTION
--------------------------------------------
The Multi Value Field Overrides (MVFO) module will add a number of 
checkboxes to any field that uses multiple values, to override 
default behavior and rendered items.

Currently offered overrides:
 * Remove table headers (useful to avoid repetition in conjunction with 
    Field Collection etc where this leads to duplication)
 * Remove tabledrag (general usability when it is not required)
 * Remove Add More button (useful when more items are added via AJAX by 
    other elements in the form)
 * Hide the empty item automatically added by core, in addition to hiding the
    add more button
 * Works with the Multiple Fields Remove Button module ( allows to hide the 
    Remove button offered )

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/mvfo

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2476509


REQUIREMENTS
------------
This module requires the following modules:
 * Fields (Core)


INSTALLATION
------------
MVFO can be installed like any other Drupal module -- place it in the
modules directory for your site and enable it on the 'admin/build/modules'
 page.


CONFIGURATION
-------------
Though there is no special configuration or permissions associated directly 
with this module, to use it you will have to edit the field you wish to enable
it for. 

Navigate to the field instance edit form of any field with multiple values,
and you should see the additional options provided by this module.

This is typically found under "Structure" -> "Content Types" -> "MyType" -> 
"Manage Fields". You can then click on "Edit" for an individual field.


MAINTAINERS
-----------
 - hexblot (Nick Andriopoulos)
