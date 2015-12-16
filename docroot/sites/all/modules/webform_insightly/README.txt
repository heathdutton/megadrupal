
Description
-----------
This module provide the integration of webforms with Insightly.

Allowded fields to map with insightly 'FIRST_NAME', 'LAST_NAME',
'BACKGROUND', 'IMAGE_URL', 'EMAIL', 'PHONE', 'BID_AMOUNT'.

CUSTOMFIELDS can also be mapped using this module. Make sure the field
name in insightly exists, while mapping (You need to add custom fields
at System Settings > Custom Fields).

Mapping can be easily handled from the admin side.


Installation
------------
1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) Visit "admin/config/insightly-mapping/add" to learn about the various settings.

-> Configure the Insightly API key.
-> Map the webforms with source as Webform fields and Target as Insightly fields.
-> 'Remove', checkbox will remove the field mapping while submitting.
-> 'Currently Mapped forms', will list at the top. From there administrators can
edit or delete the mapping.

Note: If you are dealing with custom fields, make sure the field is mandatory.

