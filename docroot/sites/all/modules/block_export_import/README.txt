CONTENTS OF THIS FILE
--------------------

Description
-----------
This module allows users to export all system specific blocks (are those blocks
which are created using Drupal interface add block functionality) and then
import it into another Drupal installation, or on the same site.

No additional configuration is required to export/Import block(s). Also this
module does not create any table in the database. Therefore it will not effect
the application performance.

Export
------
The export interface allows the user to export a single or multiple blocks with 
the following.

Export system specific block information.
Export the pages, content types, roles, users visibility settings.
Export CSS class(es) if corresponding module is installed.

Import
------
Import all system specific block information.
Import all block roles.
Import all block content types.
Import CSS class(es) if corresponding module is installed.

New Feature
-----------
Now it also support to export/import the theme region(s).

Installation
------------
1. Copy the entire block_export_import directory the Drupal sites/all/modules 
directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Access the links to to export the custom blocks 
(admin/structure/export-import-block)

3. Access the links to to Import the custom blocks 
(admin/structure/export-import-block/import)

DEPENDENCIES:
-------------
block

Support
-------
Please use the issue queue for filing bugs with this module at
https://drupal.org/node/2172541

AUTHOR:
-------
Devendra Yadav
dev.firoza@gmail.com
