README.txt
==========

This module adds a special edit view for entity reference fields, which allows
to add new nodes, respectively order and publish the appended nodes.

The edit tab is renamed to "Edit base".
If Content type is configured with a content node field, a new tab
"Edit content" will be added node view.


Howto
=====

1. Go to admin/config/content/content_nodes.
2. Select a content type, which you want to extend with content nodes.
3. Insert a Label for the new field (Later you can edit this in the field
   configuration).
4. Insert machine name (only alphanumeric characters), Fieldnames will be named
   like "content_nodes_YOURMACHINENAME".
5. After creation click on "Configure" and select your content type, you want to
   allow to append.
   - Attention: only "Node" is supported as "Target type"
6. Create new content type (see 2.) and save.
7. Click on tab "Edit content" and feel free to add new content to your node.


Rights
======

You can enable the default widget from entity reference in the node edit form:
"Access Content Nodes in node edit form"
Otherwise the field is hidden (except administrators).


Theming
=======

You can use the theme hook suggestions for nodes:
*  node--content_nodes.tpl.php
*  node--[NODETYPE]--content_nodes.tpl.php
*  node--[NID]--content_nodes.tpl.php


Notes
=====

It is not recommended, that you are using node types as content nodes, which
have content node himself. Maybe in a later version, the module can support
that. Maybe the panel module can help, to implement a complex display structure.
