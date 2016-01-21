--------------------------------------------------------------------------------
 README.txt - Blocks Placer (blocks_placer)
--------------------------------------------------------------------------------

INTRODUCTION
------------
Allows for assignment of blocks to a theme’s standard Drupal block regions, per
page, with a user-friendly interface in the context of the node edit page.

Features include:
* allows content admin to easily assign specific block(s) to a node directly
  from the add/edit node page
* provides a user-friendly UI. Content admins are able to drag-and-drop blocks
  directly into regions and filter long lists of available blocks
* restricts content admin from parts of the core block system while still
  allowing them to manage blocks. Restriction can be bypassed using permissions
* ability to hide regions directly from the node edit page
* ability to prevent specific permission level users from adding blocks to
  designated regions
* ability to insure important blocks appear at the top (menus, etc)
* ability to override global blocks right on the node edit page
* conveniently links to a sample of available block region definitions for
  content admins to review


Background
----------
This module was created to allow easy, ad hoc assignment of blocks to template block regions on a per-page basis. This allows for even the most novice of users to create unique, personalized pages full of relevant content, without having to be constrained to the rigid designs of pre-selected blocks.

At the same time, it allows for administrative control in order to maintain the site’s design standards. Administrators can disable specific regions, and “sticky” blocks to the tops of their respective regions, making sure that important features such as menus and contact information don’t get pushed out of sight.

Special Note
When the module is disabled, all field instances on all bundles (content types) will be removed, deleting all block references. Hidden regions however, are remembered if the module is enabled again.


* For a full description of the module, visit the project page:
  https://drupal.org/project/blocks_placer

* To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/blocks_placer


Detailed overview
-----------------
Node-level bean blocks:
* Provides option to place blocks in any region per node.
* Provides option to replace existing blocks in any region.
* Provides option to completely remove existing regions per node.
* Provides option to always render admin defined blocks first. Useful for
  menus for instance.
* Regions available can be limited on the settings page.

Misc:
* Adds a block config link to the Bean block admin page.

Default restrictions:
Can be bypassed with the 'Bypass block admin access control' permission
* Limits block access to Bean blocks on the core Block admin page.
* Limits regions to which global blocks can be assigned.
* Limits access to various global block form settings.
* Redirects admin/structure/block/add to Beans block/add page.


REQUIREMENTS
------------
* jQuery UI Multiselect (jQuery plugin).
  Demo: http://www.quasipartikel.at/multiselect/
  Download: https://github.com/crdeutsch/multiselect


INSTALLATION
------------
* Install as any other module, see
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information. If using Drush, make sure Entity Reference is
  installed first.

* Download the jQuery UI Multiselect plugin from GitHub. The extracted files can
  be placed anywhere in the site structure as long as the admin settings are
  updated. For auto-detection, place the files in your libraries directory at

    sites/all/libraries/jquery_ui_multiselect


CONFIGURATION
-------------
* If the jQuery UI Multiselect plugin files was placed in a non-default
  directory, update the path under "Configuration" on the admin settings page.

* Configure user permissions in Administration » People » Permissions:
  - Administer module settings
    Grants access to the modules admin settings page.
  - Bypass block admin access control
    Grants full access to the core block admin page.

* Enable Blocks Placer for the content type(s) you wish to use it for.

* Optional: Update the list of visible regions in the region list on the admin
  settings page.

* Optional: A block type with machine name 'standard' is added automatically.
  Block types used can be changed in the field settings under "Target bundles"
  for the "Blocks Placer" (blocks_placer_blocks) field.


TROUBLESHOOTING
---------------
* If installing using Drush, make sure Entity Reference is installed first.

* If regions has been disabled for a node, and then are removed from the list of
  available regions in the admin settings, then those regions will always be
  disabled for the node (as they can't be enabled in the UI). Resetting those
  regions for all nodes can be done with the following MySQL query:
  mysql> DELETE * FROM blocks_placer WHERE bid = 0 AND region = '<regionname>';
  Warning: Make sure to backup your database first.


MAINTAINERS
-----------
Current maintainers:
* enzipher <https://drupal.org/user/179782>

This project has been sponsored by:
* KNECTAR - Advanced Drupal & Magento Applications
  Visit http://www.knectar.com for more information.
