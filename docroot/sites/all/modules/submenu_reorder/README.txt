********************************************************************
                S U B M E N U  R E O R D E R    M O D U L E
********************************************************************
Original Author: Varun Mishra
Current Maintainers: Varun Mishra

********************************************************************
DESCRIPTION:

   Submenu Reorder Module provides the facility to the admin to modify
   the order of sub menu items of a node.It adds a reorder tab on node
   view page. Administrators can reorders the sub menus of a menu item
   using a drag and drop interface. They don't need to go to Menu Administration
   page. This module very usefull when you have secondary menu for each
   primanry menu items.


********************************************************************
INSTALLATION:

1. Place the entire submenu_reorder directory into your Drupal modules/
   directory or the sites modules directory (eg site/default/modules)


2. Enable this module by navigating to:

     Administration > Modules
   

3. Go to admin/people/permissions#module-submenu_reorder and assign
   permissions to the roles.


4. Please read the step by step instructions as an example to use this
   module below:-

a) Create a new node . Call it "Parent Node". Assign this node to main menu.

b) Create 3 new nodes and select "Parent Node" as Parent Item in Menu Setting
   of these nodes. Suppose we call these node as Child Node 1, Child Node 2,
   Child Node 3.

c) Now Open  "Parent node" in browser. You will see
   a "Reorder" tab along with "View" and "edit" tab.

d) Click on "Reorder" tab. You will find Child Node 1, Child Node 2,
   Child Node 3 over there. You can change their order using drag and drop.


5. You must have permission to update node. You can do this either login as 
   admin or any user who have permission to update node.

6) This module is very useful if you have lots of secondary menus of main menu 
   items. You don't need to go to menu administration page to change the order 
   of secondary menu items. If the sub menu items also have child menu items, 
   then an reorder tab will also appear there.
