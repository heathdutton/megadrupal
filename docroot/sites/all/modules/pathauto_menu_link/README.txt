ABOUT THIS MODULE
-----------------

If you use menu links to generate paths within Pathauto, you'll notice that the
resulting path is only updated when a node is saved, which means that if you
simply move a menu link item within a menu so that it has a different parent,
the path is immediately out of date and no longer correct.

This module fixes that by forcing an update of the Pathauto-generated path
when a menu link item has been updated, based of the link's new position within
the menu tree.

Should this be in the Pathauto module itself? Maybe.

MAINTAINERS
-----------

* Oliver Davies (http://drupal.org/user/381388)
