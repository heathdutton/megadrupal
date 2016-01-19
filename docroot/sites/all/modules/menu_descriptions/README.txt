Menu Descriptions (menu_descriptions)
*************************************

Ever wanted to create a menu that had more than just menu names?  This module
extends Drupal's menu system by allowing you to attach a full description to a
menu item, which can then be displayed along with the menu using menu blocks
that the module provides.  Use this to give users more direction as to what menu
options do what.  Because this is an add-on to the menu system, menu blocks obey
all access control, so that the links and descriptions that are generated are
'dynamic' depending of the user's permissions.

A new checkbox is added to the menu edit form to indicate that a Menu
Descriptions block should be generated for that menu, and a new text field is
added to menu item edit forms for the longer description.

Installation
~~~~~~~~~~~~
Install menu_descriptions like any standard Drupal 7 contrib module.  Place its
directory under sites/all/modules, and then enable the module.  Note that this
module required the Menu module to be installed.

Configuration
~~~~~~~~~~~~~
To use enable a menu for menu descriptions, edit that menu, and check the
"Create a menu descriptions block for this menu".  A new block will appear in
the blocks UI that can be place like any other block.

Then, edit each menu item for that menu, scroll down to the "Longer description"
text box, and enter your description / instruction text there.

Place your block in the region you want, with the appropriate visibility
settings for your needs, and that's it - you're done!

Theming
~~~~~~~
There are three theme functions for themers:

- theme_menu_descriptions_menu($variables('menu_tree' => array $tree))
      Themes the entire menu tree with descriptions.  The $tree array is the
      standard menu tree produced by menu_tree_all_data with a depth of one,
      and with the menu description added to each menu item.  Note the menu
      description is an object that has both the description text and the text /
      input format.  So, each element of the $tree array looks like

        [link] => array - standard link stuff,
        [below] => standard, and not used in the theming,
        [menu_descriptions] => object
            [description] => string - the description,
            [format] => string - the input / text format of the [description]

- theme_menu_descriptions_link($variables('link' => array $link))
      Called by theme_menu_descriptions_menu and themes the links.  Gets passed
      the [link] element of the menu item.

- theme_menu_descriptions_description($variables('description' => object $description, 'class' => string $class))
      Also called by theme_menu_descriptions_menu and themes the description
      portion itself.
