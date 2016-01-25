Admin Selector allows admin users with the right permissions to select between
using the Admin or Admin Menu modules. This is useful in a multi-admin environment
where different admins like different modules. Each admin user can also select
their preferred admin theme from the list of installed themes.

All code runs on permissions, so you could disable the "View admin theme" permission
to prevent users from switching the admin theme, but still allowing them to
choose their admin toolbar. The opposite can work too: disable one or both
"View toolbar" permissions, but leave the "View admin theme" one, to allow users
to only choose an admin theme.

INSTALLATION
------------
Install the module in the usual way. A new table will be created to track the
user preferences.

REQUIREMENTS
------------
This module requires both Admin (http://drupal.org/project/admin)
and Admin Menu (http://drupal.org/project/admin_menu) modules to be installed.
