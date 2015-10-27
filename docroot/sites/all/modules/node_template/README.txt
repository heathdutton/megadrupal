REDAME file for the node template module for Drupal 4.7.x

The module allows users to make a copy of an existing node and set it as a 
template for later copy. The authorship is set to the current user, the menu 
and url aliases are reset, and the words "Clone of" are inserted into the title.

Users with the "node template" permission can access this functionality. A new
tab called "set template" will appear on every node pages. Once you click this
tab you have created a new node that is a copy of the node you were
viewing, and you will be redirected to an edit screen for that new node.

This module makes reasonable checks on access permissions.  A user cannot duplicate 
a node unless they can use the input format of that node, and unless they have
permission to create new nodes of that type based on a call to node_access().

Settings can be accessed at admin/settings/nodetemplate. On this page you can
set whether an additional confirmation screen is required before making a clone
of a node, and also set whether the publishing options are reset when making
a clone of a node.  This is set for each node type individually.

This module works with common node types, but the attachments to the node 
are not included in the duplicated node. In all cases, but especially
if you are using a complex or custom node type, you should evaluate this
module on a test site with a copy of your database before attempting to use
it on a live site.

To install this module, copy it to the /modules directory of your Drupal 
installation and enable it at /admin/modules. A new table will be created to store 
the information of node templates.