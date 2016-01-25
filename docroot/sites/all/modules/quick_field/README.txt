
-- SUMMARY --

The Quick Field module allows site admin to create / clone fields to any 
content type in a fast and efficient manner with minimal configurations.
This module checks for field settings for a particular field type and shows 
these settings on the same creation page.

Also, admin can clone an existing field within same content type.


-- REQUIREMENTS --

field_ui


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:

  - Use the administration pages and help (System module)

    The top-level administration categories require this permission to be
    accessible. The administration menu will be empty unless this permission is
    granted.

  - Access administration menu

    Users in roles with the "Access administration menu" permission will see
    the administration menu at the top of each page.

  - Display Drupal links

    Users in roles with the "Display drupal links" permission will receive
    links to drupal.org issue queues for all enabled contributed modules. The
    issue queue links appear under the administration menu icon.

  Note that the menu items displayed in the administration menu depend on the
  actual permissions of the viewing user. For example, the "People" menu item
  is not displayed to a user who is not a member of a role with the "Administer
  users" permission.

* Customize the menu settings in Administration » Configuration and modules »
  Administration » Administration menu.

* To prevent administrative menu items from appearing twice, you may hide the
  "Management" menu block.


-- TROUBLESHOOTING --

* If the menu does not display, check the following:

  - Is the "Administer Quick Field" permission enabled for the 
    appropriate roles?


-- CONTACT --

Current maintainers:
* Gaurav Pahuja (gaurav.pahuja) - https://drupal.org/user/1804170
* Pushpinder Rana (er.pushpinderrana) - https://drupal.org/user/1004418

This project has been sponsored by:
* Sapient Consulting
