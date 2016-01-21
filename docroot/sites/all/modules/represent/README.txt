Description
-----------
The Represent API Module only provides a mechanism for module developers to
query the Represent API [1].  It is mainly intended as a launch point for
developers to create plugin modules that harness the Represent API.

Represent is the largest database of elected officials in Canada, and offers
the most comprehensive postal code lookup service for elected officials at the
federal, provincial and municipal levels of government.

1. https://represent.opennorth.ca/

Installation
------------
1. Copy the entire represent directory to the Drupal sites/all/modules
   directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Site
   Building" -> "Modules"

Troubleshooting
---------------
The Represent module will cache Represent API responses for a week. The cache
may be cleared by flushing Drupal's core cache tables.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/represent
