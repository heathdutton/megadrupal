Menu Block Settings Override
============================

This module allows for overriding the settings of a Menu Block, or hiding
a Menu block on specific nodes.

The default settings of a Menu block can be set at:

admin/structure/block/manage/menu_block/DELTA/configure

In order to override these default settings for a Menu block on specific pages,
the steps to follow are:

1. Apply the following patch to the Menu block module:
   
   https://www.drupal.org/node/2377995
   
2. Assign the "Override Menu block settings" permission to all users who ought
   to be able to override these settings on specific pages.
   
3. Check all the Menu blocks that you want to make configurable on a per-page
   basis in the settings page for this module:
   
   admin/config/user-interface/menu-block-settings-override
     
4. On this same settings page you can pick the settings which these users
   don't need to see while editing a page.
   
5. Users with the appropriate permission can now override the settings for these
   blocks when editing a node, by checking "Override Menu block settings on this
   page" or "Hide Menu block on this page" under "Menu block settings".
