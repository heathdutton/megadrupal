
Domain Menu Access is an extension to Domain module, allowing
administrators to configure visitors' access to selected
menu items based on current domain they are viewing.

It lets administrators decide whether a specific menu item
should be hidden on selected domains (regardless of it being enabled
by default using standard Drupal functionality), or should it be
displayed on selected domains even if disabled by default.


Installation
------------

 * Download/checkout the domain_menu_access folder to the modules folder
   in your installation.

 * Enable the module using Administer / Modules (/admin/modules).

 * Grant 'Administer menus per domain' permission to relevant
   admin users using Administer / People / Permissions
   (/admin/people/permissions)


Usage
-----

Access to all menu items is managed on standard admin "Edit menu item"
pages in Admin / Structure / Menus, separately for each menu item,
using domain checkboxes in "Manage item visibility per domain" fieldset.

Use "Show menu item" checkboxes to force displaying items which by default
are disabled by Enabled checkbox in Menu settings section.

Use "Hide menu item" checkboxes to force hiding items which by default
are enabled.

Note that these settings will be ignored in administration area,
which means that all menu items will be enabled or disabled based
on default Drupal settings only, as otherwise default state
of a menu item could be changed by accident.


Author/maintainer
-----------------

Maciej Zgadzaj
http://drupal.org/user/271491