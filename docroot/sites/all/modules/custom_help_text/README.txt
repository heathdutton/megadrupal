DESCRIPTION
--------------------------
This module integrates into the Help module (using the hook_help() hook) to add 
help messages based on the URL. There is an administration form that allows 
users to set the messages and where they appear on the site. Multiple help 
messages can be added to the same page at once, the administration form can be 
ordered to sort the order that these messages appear.

In order to make the help messages appear you need to make sure that the 
$messages variable is printed on your template layer. Most themes will do this.

You can also allow certain roles to be able to view the help items by selecting 
them on the hook item add/create form.

FEATURES
------------------------
The module is integrated into Features via the use of the CTools module.

UNIT TESTING
----------------------
This module comes with a few unit tests that cover all of the existing 
functionality of the module.

CREDITS
----------------------------
Authored and maintained by Philip Norton <philipnorton42@gmail.com>
Thanks for digital006, klausi and beanluc for his help in getting this module
finished.
