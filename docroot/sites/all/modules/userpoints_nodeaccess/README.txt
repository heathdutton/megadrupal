$Id


-- SUMMARY --

The Drupal userpoints nodeaccess module enables you to sell a single 
node for a specific category and amount of userpoints.


For a full description of the module, visit the project page:
  http://drupal.org/project/userpoints_nodeaccess

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/userpoints_nodeaccess


-- REQUIREMENTS --

* Userpoints module
  http://drupal.org/project/userpoints


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> 
  Permissions >> userpoints_nodeaccess module:

  - administrate userpoints nodeaccess

    Users in roles with the "administrate userpoints nodeaccess" permission 
    will be able to change the settings of this module.
    
  - buy access to a node
  
    Which roles are able to buy access to a node?


  Note that the superuser (user 1) always will see the node's, even without 
  buying access to it.

* Customize the menu settings in Administer >> Site configuration >>
  Userpoints Node Access.
  
  - Show a message when the user already bought access to the node and 
    tries to buy it again. (Y/N)
    
  - Default points category


-- CONTACT --

Current maintainer:
* Tom Behets (betz) - http://drupal.org/user/232832



