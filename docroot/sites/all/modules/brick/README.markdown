Brick
=====

This module uses ctools context, content types, access and export to create a
small, efficient extension of the Drupal 7 core block system. The project
includes some examples that demonstrate the basic functions.

Requirements
------------

* Drupal 7
* ctools

Usage
-----

After enabling the module and its dependencies, you can immediately begin
creating bricks. Once created, these bricks will appear in the block admin page
as blocks that you can enable and place in regions like other blocks.

1. Visit the admin UI at /admin/structure/brick
2. Add a new brick.
3. After the basic settings, the wizard asks for "context." If you've used
   Panels, you will be familiar with context. If you want your brick to react to
   a node, term or user on the page when it appears as a block, you must add
   a required context. Relationships allow you to bring in other contexts based
   on the required contexts: a term from the node, the user who authored a node,
   etc.
4. Configure the visibility settings. If you added contexts in the previous
   step, you can use those to determine visibility here.
5. Choose a content type and subtype. List of choices is limited by the contexts
   you chose previously. If you modify the contexts for this brick, the valid
   options for the content type will change.
6. The configuration of the content type depends on the content type, and it may
   be multiple steps depending on the content type.
7. When your brick is saved, visit the blocks admin UI /admin/structure/block
   and add the new bricks to regions.
8. If you are adding a brick with a required context, you must enable a task in
   Page Manager which creates an argument context to match. For example, enable
   `node_view` so that your brick can appear on `/node/%node` pages. Even if you
   do add any handlers for this task, you must enable the handler for your
   bricks to receive their required contexts.

Review the known issues section below for workarounds.

Known Issues
------------

The select element for the required context shows multiple choices for what
appears to be the same kind of required context. Only one of the listed
choices is valid. The second "node" context listed is the valid one.
