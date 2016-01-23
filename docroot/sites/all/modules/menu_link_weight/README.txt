INTRODUCTION
------------

The Menu Link Weight module replaces the standard numeric weight dropdown widget
for menu links in the node form provided by Drupal Core with a tabledrag widget.

Upon selection of a parent, a tabledrag widget will be loaded via AJAX that
will allow you to reorder the weight of the menu link for the node you are
editing relative to its sibling links. You can also reorder the sibling links
themselves if you like. There will be graceful degradation if Javascript is not
available.

Upon node submission, the weights of all the children of the selected parents
that the user has access to will be internally reordered from -50 to -49, to
-48 etc. Note: this will overwrite existing weights for children of the
selected parent item!

With "row weights" hidden, content editors can now ignore the numerical weight
values and instead see the position of a menu link relative to other links in
the tabledrag widget itself.

This module includes support for the Hierarchical Select module
(https://www.drupal.org/node/172915/).


REQUIREMENTS
------------

No special requirements.


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------

Configure user permissions in Administration » People » Permissions:

 * All users with the "Administer menu" permission will now have access to the
   Menu Link Weight widget when adding/editing a node.
