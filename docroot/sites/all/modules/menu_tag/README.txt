DESCRIPTION
-----------

The menu_tag module allows you to control the display of menu items through a 
settable tag of a single menu item. Then, for every block that displays a menu, 
you can select a tag that filters what menu items should be displayed. For 
instance, you can use this method to control the visibility of menu items in 
blocks that display Main and Secondary menu on some specific page.

This module is fully compatible with http://drupal.org/project/menu_block/
(menu_block module) which is used to create configurable blocks of menu trees 
starting with any level of any menu.

The main benefit of combining menu_tag with menu_block is that you can create 
one huge menu tree with large number of menu items and then reuse different 
subsets of this menu tree. For instance, on a specific web page or on 
specific section on your site, you can create a block displaying only a 
small subset of Main menu items and then make that block though theming 
an eye-catching one.

This problem was solved in the past by duplication of menus and menu items; 
that, however, produced overhead in maintenance of a web site.

Development of this module was sponsored by MD-Systems (http://md-systems.ch/). 
Development, Documentation & Community Support is delivered by MontenaSoft
(http://montenasoft.com/). 


CONFIGURATION STEPS
-------------------

- Install as usual.

- Create new menu (/admin/structure/menu/add). 

- Add some menu items.   
  
  (Note: to be capable of seeing the filtering results, we advise that all 
   menu items that contain child menu items should also have set the flags
   "Enabled" and "Show as expanded")    

- Define menu tags for newly created menu items, by adding <b>ONLY ONE</b> 
  tag per menu item. You can use one of following options for this task: 

  * menu edit (/admin/structure/menu/manage, and then "edit menu")  
  * menu item (on menu item edit form, by editing "Menu tag"), or 
  * menu section on node edit form.


- Open menu block configuration form (/admin/structure/block/list and 
  eventualy select the theme) and add menu block to a visible region. Save 
  configuration.  

- Visit again theme's block confiuration page, locate again you menu block and 
  press "Configure" (/admin/structure/block/manage/menu/"your menu"/configure) 

- Select option in "Menu tag filter". There are two preconfigured options:

  * "Don't apply meny tag filter": all available menu items will be displayed 
  * "Display only items without menu tag": only items without entered value 
    for menu tag will be displayed

  followed by list of menu tags that exists in your website. 


This procedure applies for all menus that are added to a Drupal website. 

Since Main and Secondary menu are treated specially by the Drupal itself, 
the menu_tag settings for this menus can be done through: 
/admin/structure/menu/settings


If you would like to create several blocks that are based on one menu, and then 
use menu_tag module to filter the output, use the menu_block module 
http://drupal.org/project/menu_block/ to create several menu blocks and 
filter their content through selection of specific menu_tag. 
