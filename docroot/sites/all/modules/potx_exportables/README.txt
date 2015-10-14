INTRODUCTION
--------------------------

This module aims to ease the deployment of translation .PO files that the great Potx module generates. No longer will you have to manually upload these files to keep your site translations up to date. You can manually batch import all PO files defined in custom modules or  alternatively, you can use the always useful Drush command to make things easier for you.

All you need to do is implement hook_file_location_info to let this module know where it can find the PO files and then it will take care of the rest.

As a side bonus, you will be able to version these PO files and keep changes in code.

Features:
- Automatically imports PO files defined in custom modules.
- Only newly found or updated PO files will be processed.

Features yet to do:
- Add support for text groups. So far this module only supports the default group.

Be aware:
-   Strings found in PO files will replace existing ones and new ones will be added.
Requirements: Views.

CONFIGURATION
--------------------------

This module requires no configuration. Just install and enjoy.


INFORMATION FOR DEVELOPERS
--------------------------

Developers can define the location where the module should
find PO files by implementing hook_potx_file_location_info(). 

Here is an example implementation:

function test_module_potx_file_location_info() {
  return array(
    'test_module' => array(
      'path' => drupal_get_path('module', 'test_module') . '/potx',
    ),
  );
}
