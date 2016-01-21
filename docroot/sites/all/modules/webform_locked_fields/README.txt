
CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation

AUTHOR
------
Sheldon Rampton ("Sheldon Rampton", http://drupal.org/user/13085)

DESCRIPTION
-----------
This module provides a way of locking webform fields so they can only be edited
by users with permission to do so. In combination with the webform_default_fields
module, this makes it possible to create default fields on webforms that cannot
be removed or modified. For example, you might want to require all webforms to
contain standard fields for name, address, etc.

INSTALLATION
------------
To use this module, install it in a modules directory. Enable the
"Administer webform locked fields" permission for user rules that should be
able to edit and/or delete locked webform fields. When editing a webform field,
users who have this permission will see a "Lock this component" checkbox. If
that box is checked, further revisions to that webform field will be restricted
to users who have permission to do so. Users without that permission who attempt
to edit or delete a locked field will see a "Not allowed" page.