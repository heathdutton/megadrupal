INTRODUCTION
------------

The Menu Link Sync module helps synchronize the position of menu items within
Drupal installs with separate menus for every language, for instance when we have
different "Main Menu - French", "Main Menu - English" and "Main Menu - Spanish"
menus.

This synchronization may be useful when the structure of the menus is too
different for i18n_menu or Entity Translation to be adequate solutions, but when
the trees for different languages are still similar enough for some sort of
synchronization of menu structures to be desirable.

This module provides a "Synchronize" button to the "Menu link settings" on the
node forms for translated nodes. By pressing the "Synchronize" button, the
parent and the relative tree position of the menu link for this translated
node are automatically calculated to be as close as possible to the parent
and relative position of the menu link for the "source" node. The form will
then be updated through an AJAX call and the new parent and weight will be
automatically selected.


REQUIREMENTS
------------

* Menu Node API: https://www.drupal.org/project/menu_node


RECOMMENDED MODULES
-------------------

* This module integrates with Menu Link Weight:
  - https://www.drupal.org/project/menu_link_weight

  This integration allows us to synchronize the relative  position of a menu link
  within a menu tree (ie. "underneath item X" / "above item Y"), instead of
  simply copying the absolute numeric weight of the menu link from the source
  node.

* This module integrates with Hierarchical Select:
  - https://www.drupal.org/project/hierarchical_select

  The Hierarchical Select Menu module allows for easier management of large menu
  trees.


COMPATIBILITY
-------------

Please note that synchronization will currently not work for nodes that are
translated using Entity Translation. Only Content Translation is supported
at this point!


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------

No special configuration is needed.
