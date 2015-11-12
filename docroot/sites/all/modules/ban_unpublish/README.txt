This module creates a bulk operation for quickly unpublishing
all the comments and nodes created by a user. It's particularly
useful for cleaning up after spammers.

Installation

1. Place the ban_unpublish folder in the modules directory of your 
Drupal installation.

2. Enable the module at admin/build/modules.

3. If you're using Views Bulk Operations with the Actions Permissions
module, grant permission to use the operation at admin/user/permissions.

Operation

"Block, ban and unpublish all content by the selected users" will be
available in the dropdown at admin/user/user and for any additional
user-management modules, such as Advanced User. 

If you create any Views Bulk Operations views of user data, it also 
will appear there.

Always be careful about VBO permissions to administrative functionality
that you might create.

Credits

Steve Yelvington (steve@yelvington.com) is the author and maintainer 
of this module. Some code was borrowed from the Abuse module, and 
sirkitree's post on implementing bulk operations was helpful.
http://sirkitree.net/node/23

Initial D7 port: Issue #1938428 by SoumyaDas: Drupal 7 version of 
Ban and Unpublish module

