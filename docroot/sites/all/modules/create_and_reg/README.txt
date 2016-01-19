
-- SUMMARY --

This module creates a "lazy registration" which means that the user creates the
content first and after this logs in or registers to the site.

For a full description of the module, visit the project page:
  https://drupal.org/project/create_and_reg

To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/create_and_reg


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:

  - Administer create and register

    Users in roles with the "Administer create and register" permission will
    access the administration form.

  - Create node

    You have to add the create content type permission for the selected content
    types for anonymous users.

* Set the selected content types to unpublished by default.

* Configure the settings in Administration » Configuration » Workflow.

  - Select the node types where you want to use the registration method.

  - Select the redirect method.

  - Disable other status messages if you want.

  - Add a custom status message if you want. You can use replacement tokens if
    the token module is enabled.


-- CONTACT --

Current maintainers:
* Kálmán Hosszu (kalmanhosszu) - https://drupal.org/user/267481
