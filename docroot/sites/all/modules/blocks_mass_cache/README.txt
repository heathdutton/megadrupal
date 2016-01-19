Module: Blocks Mass Cache

Description
===========
Fine tuning for blocks cache. It is possible to select the
type of cache for each block.

Installation
============
Copy the module directory in to your Drupal:
/sites/all/modules directory as usual.

Enable Blocks Mass cache and Blocks Mass Cache Admin UI
for development environments.

Configuration
====================================================
Visit /admin/structure/block/mass-cache
or via menus: Structure > Blocks > Mass Cache

Tick the boxes. It's possible to have combinations of
Role + Page / User + Page.

Make sure that "Cache Blocks" is enabled on
Configuration > Development > Performance

IMPORTANT: In Drupal, blocks are not cached if you are logged in 
as admin (user id 1), so make sure you test blocks caching with 
an user different than user 1 or as anonymous.
