Description
-----------
This module adds two new components to your webforms allowing you to group
fields together inside of a jQuery UI Accordion control
(http://jqueryui.com/demos/accordion).

Requirements
------------
Webform 3.x


Installation
------------
1. Copy the entire webform_accordion directory the Drupal
   sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Site
   Building" -> "Modules"

Creating an accordion
---------------------
1. Create or edit a webform node at node/add/webform.

2. Add one or more Accordion Containers to your form.
   Each container represents a single Accordion control.

3. Add two or more Accordion Tab controls to your form as children of an
   Accordion Container.

4. Add Webform controls as children of each Accordion Tab.
