CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installation
 * Instructions
 * Design Decisions


INTRODUCTION
------------
Current Maintainer: franksj

Views Fields On/Off is a module containing a Views field that allows users to
selectively show or hide fields at view-time.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/franksj/2392641

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2392641


REQUIREMENTS
------------
This module requires the following modules:
 * Views (https://drupal.org/project/views)


INSTALLATION
------------
Install as usual, see http://drupal.org/node/895232 for further information.


INSTRUCTIONS
------------
In a View with fields, Add a new field Global: On/Off Form, which allows
display fields (fields not excluded from the display) to be switched on or off.
In the exposed form area of the View display, a list of checkboxes for each
selected field will appear. The user can check the fields to show or hide and
apply it, then the field will be displayed or not displayed when the View
displays! The selection of hidden fields will also exclude the de-selected
fields from Views Data Export.


DESIGN DECISIONS
----------------
This Views Field Handler is patterned off of the coding standards as used in
the Views module. Views classes are not camel-cased and they do not have
function visibility defined.

Since the On/Off handler is not a real field, it implements an empty query()
method so that the parent views_handler_field::query() method isn't called.
