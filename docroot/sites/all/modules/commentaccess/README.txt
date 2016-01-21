
--------
Overview
--------
Comment Access gives you more control over comment permissions. With this module your users can administer, approve, and delete comments on nodes they create without giving them full comment administration access. Permissions are on a per node type basis, so it's a great way to, e.g., allow users to manage comments on their own blogs.

------------
Requirements
------------
This module requires Drupal 7.x and the comment module enabled.

------------
Installation
------------
1. Copy the commentaccess directory and its contents to the Drupal modules directory, probably /sites/all/modules/.
2. Enable the module in the modules administration page.
3. Enable the appropriate permissions on the people administration page, on the permissions tab.
4. Update the default approval email on the usercomment admin page /admin/config/content/commentaccess.

---------------------------
Upgrading from User Comment
---------------------------
If you are upgrading your site from Drupal 6.x and have been using the User Comment module, simply uninstall that module first before following the installation instructions above.

--------
Settings
--------
There are some user settings available on the user edit form.

-------
Credits
-------
Updated for Drupal 7.0 by Ryan Schwab.
Adapted from User Comment by Gwen Park.
