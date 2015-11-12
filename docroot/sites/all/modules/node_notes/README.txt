DESCRIPTION
-----------
This module provides a private simple commenting system for nodes. It is
intended to help administrators, sales staff, or whatever to leave notes on a
node without tying up the core comment module on the same node type.

It has it's own set of permissions but it does not, in this current version,
allow for permissions to be configured per node type.

Once enabled and attached to a node type, the module adds a "notes" tab to the
specified nodes visible to permitted users.


Installation
------------
Enable the module as normal.

Navigate to admin/structure/types/manage/%type. Select the "Node Notes" vertical
tab and tick the checkbox.

Navigate to admin/people/permissions to assign appropriate permissions to each
role on your site.
