Description
-----------
Current module is the extension of the basic drupal_sync module.
It allows to synchronize content of any menu with multiple websites.

Installation
-----------
1) Enable the module. Then go to Settings page- admin/config/drupal_sync/settings
 on all sites that should be synchronized. Select all menu types being
synchronized  in "SYNC ENTITIES" section.
2) "Synchronization settings"  layout will appear on Menu edit
pages-admin/structure/menu/manage/%menu-name.  Here you can finish setting up
synchronization by customizing outer websites.

While menu links customization it’s necessary to ensure that the path on target
site is the same as on source site. Identification nid for links of “ node/%nid”
type will change on local nid on distant site.  The synchronization for these
nodes should be customized as well. Link with outer paths (for example: http://google.com)
should not be changed.

