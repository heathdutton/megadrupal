Description
-----------
This module add own component type for select options. This select options you
construct from avalable fields per content types. The text (label) of options
is node title, option value may by any other field of enabled content types -
recommended textfield no-multiple.

Requirements
------------
Drupal 7.x


7.x-1.x
Installation
------------
1. Copy the module directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Edit the settings under "Administer" -> "Configuration" ->
   "Content authoring" -> "Webform settings" -> "Webform Select options
   settings"
   
   3.1. Enable content types from which fields you want construct Select options
   admin/config/content/webform/wfso
   
   3.2. Choose options Value field for each enabbled content type
   admin/config/content/webform/wfso/fields_setting

4. Create a webform node at node/add/webform. Add component type "textfield".
  Choose the SELECT OPTIONS on the bottom of component edit page - previously
  configured in step 3.

7.x-2.x
Ability for using field with email addresses as Value. This options prevent to
expose emails on front-end.

7.x-3.x
Created this feature as component. Avalable in "E-MAIL TO" webform settings.

Step 4 changed to:

4. Create a webform node at node/add/webform. Add component type
  "WF Select opts field" (wfso_field). Choose the SELECT OPTIONS on the bottom
  of component edit page - previously configured in step 3.

5. In "E-mails" tab select "Component value" and choose created component field,
   click "Add" and save.

You can set a default value for WF Select opts field by value.
Example:

   If rendered options are:
   
   <option value="1">One</option>
   <option value="2">Two</option>
   <option value="3">Three</option>
   <option value="4">Four</option>
   <option value="5">Five</option>
   <option value="6">Six</option>

   <a href="/node/nid?default=6">Link text here</a>

      node/nid - webform path or alias
      6 - value of options to be chosen


ToDo
----
Create docs node with examples.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/webform_select_opts

Author
------
Svetoslav Stoyanov
http://drupal.org/user/717122