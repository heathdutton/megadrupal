Description
-----------
This is a general purpose module that allows users to unsubscribe from all
email communications sent by Drupal.

The module design is very basic. When a user unsubscribes, they are added to an
"unsubscribe list," which is stored in the database. Whenever Drupal sends 
mail, the unsubscribe module will check to see if the recipient is on the
unsubscribe list. If so, the email is blocked. You can exempt specific modules
from this blocking via the configuration page. You can also use hooks to alter
exemptions, or override the blocking.

Requirements
------------
Drupal 7.x

Installation
-----------
Download the module and enable it!

API Integration
---------------
The unsubscribe module provides an API for modifying module exemptions,
overriding email blocking, and reacting to unsubscribe events. 

More information about Unsubscribe's hooks can be found in
unsubscribe.api.php file included with this module.

Theming
-------
The unsubscribe module allows for the unsubscribe link to me themed
via theme_unsubscribe().

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/unsubscribe?categories=All

Author
-------
Written by Matthew Grasmick (@madmatter23).

