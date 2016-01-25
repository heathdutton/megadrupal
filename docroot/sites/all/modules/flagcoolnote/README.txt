Flag Cool Notes
------------------------

Description
-----------

This module provides the Drupal admin to attach a webform to a flag so that when
users flag/unflag content on the Drupal site, they can add additional information 
using the webform.

The administration section of this module allows the Drupal admin to Add/Edit/Delete
FLAG COOL NOTES.

Permissions
-----------

The Drupal admin can set permissions to user roles, who can add/edit/delete these 
"FLAG COOL NOTES" and attach any webform to a Flag. The permission is to 
"manage flag fields" and a user who has this permission can manage these settings.

Installation
------------

The "FLAG COOL NOTES" module attaches webforms to flags, hence depends on the Flag module 
and the WebForm module. Ensure that these 2 modules are installed and enabled on the site.
To install, place the entire flagcoolnote folder into your site's modules directory.
From the Drupal administration section, go to Administer -> Modules 
and enable the Flag Cool Notes module and its dependencies.

Updates
-------

This version adds support for views and separate the tests cases from module root to tests directory.
To update follow steps below:
1. Fixes for #1819792
2. Added support for multi-step web-form

Configurations
--------------

To add a custom form on a flag form, you will need to map the required flag form with an existing webform.
To do this,
1. Go to Administer -> Structure -> Flag Cool Notes Settings.
2. Click on Add tab
3. Create a flag field using the select list.
 3.1 Select Flag name from Flags select list
 3.2 Select flag action from Flag Action select list
 3.3 Select webform name from Webforms select list
4. Click on Save.

Once you have created a new flag cool note (e.g. "offensive"), it is time to try it out.
Go to content where there is a flag link to mark that content as flagged.
Click on that link and it will take you to the confirmation page where you can find the webform to fill.
Fill that webform and click on submit.


Known incompatibilities
-----------------------

The module currently only works with confirmation form link settings of flag.

Issues
------

If you have any concerns or found any issue with this module please don't hesitate to add them in issue queue.

Maintainers
-----------
The Flag Cool Notes was originally developed by:
Yogesh S. Chaugule

Current maintainers:
Yogesh S. Chaugule
