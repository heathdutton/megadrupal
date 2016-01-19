
Language Menu Access is a modified version of the Domain Menu Access module.
It has been adapted to work with languages instead of Domain Access domains.
The reason for creating this module was the requirement of enabling/disabling
menu items. This can be done by localizing the menu through the i18n_menu module, 
but each time a menu item is translated a new menu item is created.  This makes
it difficult to keep a menu structure consistent, and will also break anything
that relies on mlid's (e.g. the Menu Block module).

The rest of this README file is copied from domain_menu_access - with every occurance
of "domain" replaced with "language" :-)
========================================

It allows administrators to configure visitors' access to selected
menu items based on current lanaguage they are viewing.

It lets administrators decide whether a specific menu item
should be hidden on selected languages (regardless of it being enabled
by default using standard Drupal functionality), or should it be
displayed on selected languages even if disabled by default.


Installation
------------

 * Download/checkout the language_menu_access folder to the modules folder
   in your installation.

 * Enable the module using Administer / Modules (/admin/modules).

 * Grant 'Administer menus per language' permission to relevant
   admin users using Administer / People / Permissions
   (/admin/people/permissions)


Usage
-----

Access to all menu items is managed on standard admin "Edit menu item"
pages in Admin / Structure / Menus, separately for each menu item,
using language checkboxes in "Manage item visibility per language" fieldset.

Use "Show menu item" checkboxes to force displaying items which by default
are disabled by Enabled checkbox in Menu settings section.

Use "Hide menu item" checkboxes to force hiding items which by default
are enabled.

Note that these settings will be ignored in administration area,
which means that all menu items will be enabled or disabled based
on default Drupal settings only, as otherwise default state
of a menu item could be changed by accident.


Author/maintainer (of original domain_menu_access module)
-----------------

Maciej Zgadzaj
http://drupal.org/user/271491