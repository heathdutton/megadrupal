PER-NODE COMMENT MODERATION
========================================

Description
------------------
This is a very simple, lightweight module that aims to solve the problem
of providing comment moderation on a per-node level. Drupal provides
a site-wide permission to determine which users can post comments that
do not require approval. In certain use-cases, you may want to only 
enforce that comments require approval on certain nodes.

The module uses Flag to toggle for each node. Only node types that have
this flag active will be affected (otherwise, global permissions will
dictate comment status).

Dependencies
------------------
- Comment (core)
- Flag

Installation & Configuration
------------------
1. Install the module and dependencies as you normall would.
2. Edit the node_comment_moderate flag (@admin/structure/flags) to configure
   which node types to attach this flag to, which roles can use it, etc.
3. Toggle the flag to enable comment moderation for that given node. If
   the node is currently flagged, all comments (for non-admins) will be 
   unpublished when created. If the node if not flagged, all comments 
   will be pubished regardless of site-wide permissions.

Upgrading from Drupal 6.x
------------------
There is currently no upgrade path from Drupal 6
