-- SUMMARY --

The Masqueradedit module is an add-on to the Masquerade module that allows a
user with the correct permissions to edit a node as the user who created it.

-- REQUIREMENTS --

The Masquerade module must be installed and configured, with the permission to
Masquerade as a user granted.

-- INSTALLATION --

* Install as usual, see https://drupal.org/node/895232 for further information.

-- CONFIGURATION --

There is no configuration for this module, but the Masquerade module needs to be
configured as per its instructions.

-- CUSTOMIZATION --

See the API file in this module.

-- TROUBLESHOOTING --

If the "Edit as user" menu tab does not appear, try the following:

- Clear all caches.

- Ensure the Masquerade permission 'masquerade as user' is granted to the role
  of the user trying to access.

- Ensure that the node is not created by the user trying to masqueradedit it.

- Ensure that the user is not currently masquerading.

-- FAQ --

Q: How do I change the title of the Masqueradedit button?

A: If you use hook_menu_alter() on the $item['node/%node/masqueradedit'] you can
   set a title or a title callback.
   See https://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_menu_alter/7
   for more information.

Q: How do I add a custom "Save and switch back" button to a form?

A: The code for a Masqueradedit switch submit button is in the
   masqueradedit_form_alter() function, and can be added to any form using
   hook_form_alter().

-- CONTACT --

Current maintainer:

* Dale Smith (MrDaleSmith) - https://drupal.org/user/2612656

This project has been sponsored by:

* CTI Digital
  Since 2006 CTI Digital's Drupal team have delivered customer experiences that
  delight and inspire. As one of one of Europe's leading agencies we specialise
  in providing end to end solutions to enterprise clients.
  https://drupal.org/node/2104891
