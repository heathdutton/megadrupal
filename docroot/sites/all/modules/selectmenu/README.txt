jQuery UI Selectmenu
====================

Description
-----------
This module provides a Drupal integration for an improved version of the jQuery
UI selectmenu widget originally developed by Scott of the filament group.

Select lists are often difficult to theme because browsers only allow certain
parts of the element to be changed (the border or background color for example).
Therefor it is difficult to have highly customized select lists that use
background images, HTML, or effects of any kind on the list.

The jQuery UI Selectmenu library avoids these problems by providing an
accessible and ARIA supported alternative based on JavaScript. Users without
JavaScript will fallback to the original select element.

Requirements
------------
Drupal 7

This module depends on jQuery UI, but uses the bundled copy that comes with
Drupal core. It will add the necessary JS files to the page when needed.

Installation
------------
1. Copy the entire selectmenu directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module under "Modules".

3. Configure the module at "Administer" -> "Configuration" -> "User Interface"
   -> jQuery UI Selectmenu.

   By default jQuery UI Selectmenu will affect all front-end forms, but not
   those within the node forms, system settings forms, or in the Overlay module
   administrative overlay. You may enable the select list enhancement based on
   your preferences.

   You may also disable the Selectmenu behavior on any individual form by
   entering the ID attribute of a particular form. You can get this ID by
   viewing the HTML source of the page and reading it from the <form> tag.

Support
-------
Post support requests to the module issue queue at:
http://drupal.org/project/issues/selectmenu?status=All

Please search the existing issues before posting new requests.

