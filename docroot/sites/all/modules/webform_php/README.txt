Description
-----------
This module adds the ability to execute arbitrary PHP code when a Webform is
validated or submitted.

This module is extremely dangerous and you should not use it. By using this
module you enable any user with the "use PHP for additional processing"
permission to wreck absolute havok on your site, deleting files or dropping
your entire database.

This module does serve as a good example of how you can write your own custom
module and add any extra code you may want to execute. No support is provided
on how to write your own custom routines. See the Webform handbook pages for
information on how you can use these fields:

http://drupal.org/handbook/modules/webform/validation-code
http://drupal.org/handbook/modules/webform/submission-code

Requirements
------------
Webform module

Installation
------------
1. Copy the entire webform_php directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Site
   Building" -> "Modules"

3. Enable the permission for "use PHP for additional processing" under
   "Administer" -> "User management" -> "Permissions"

4. Create or edit a webform node. Additional processing fields will be available
   under the "Webform" -> "Form settings" tab.

Support
-------
Absolutely no support is provided for any code that you may execute in these
fields. Please do not submit any questions about how to use this module to
the Webform PHP issue queue (or to the Webform module issue queue).

