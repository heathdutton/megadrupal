-- SUMMARY --

This module provides an easy way to indicate which menu item is active adding 
the active CSS class depending the path that the users are seeing.

IMPORTANT
This assumes they are using theme('menu_link') for the menu rendering to html.

-- FEATURES --

* Configuration interface to enter the global CSS classes that will be 
applied to the active menu item
* Configuration interface for each menu item (under the fieldset 
"Custom Active Menu Item") to provide the path where will be active
* Provides a permission setting to allow administrators manage the features 
mentioned above
* Is complatible with nice_menus module
* Works with path aliases

-- REQUIREMENTS --

* menu module enabled

-- INSTALLATION --

* Copy the 'cami' module directory in to your Drupal
sites/all/modules directory as usual.

-- USAGE INSTRUCTIONS --

* Activate the module "Custom Active Menu Item"
* Configure each menu item that you want. In "Custom Active Menu Item" fieldset 
there is one field to input all paths where the menu item should be active
* You can configure what class should be added to the active menu item in 
admin/config/user-interface/custom-active-menu-item
* Setting up the permission 'administer cami' if you want

-- CONTACT --

Current maintainers:
* Gerardo Daniel Cadau (juanramonperez) - http://drupal.org/user/549414
