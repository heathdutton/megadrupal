Codit: Blocks
=============

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * Building Blocks
 * Drush Commands
 * FAQ
 * Roadmap
 * Bonus Features
 * Similar Projects
 * Maintainers

INTRODUCTION
------------
Codit: Blocks allows for creation of simple or complex blocks with custom
callback, access, and caching available for each block.  Like all Codit based
modules, this is done entirely through code and no admin.

FEATURES
--------
 * Blocks can be added quickly through code by adding a block directory and
   template, then flushing cache. Or use a drush codit-blocks-add.
 * Block Callback Function - Used for doing any heavy processing for the block.
   The code is contained in an anonymous function to reduce collision risk.
   Anything returned by the function gets passed to the template.
 * Access Function - Used to restrict access based on specific permission, role,
   user, field value or any combination thereof.
 * Caching - Caching of any Codit: Block's block can be done on a site-wide,
   per page, per path, per user, per role, per permission, per field value or
   any combination thereof.
 * Any block placed on a node or taxonomy term page has immediate access to that
   node object or taxonomy term.
 * Can be used to override any block title, including the ability to disable all
   block titles except any that have been overridden.
 * Block placement can be handled by the Context module, Panels, or the block
   admin.

REQUIREMENTS
------------
Dependency: Codit: Local
Dependency: Codit

INSTALLATION
------------
 * If Codit and Codit: Local have already been installed on your site, then this
   submodule is ready to be enabled.  Once enabled, if you use drush to create
   your blocks, then you need to do nothing more.
 * If you do not use drush to add a block, then you must copy the contents of
   the codit_block/boilerplate into codit_local

CONFIGURATION
-------------
* No configuration is needed.  You simply create blocks and position them with
  Context, Panels, or the native block admin.

BUILDING BLOCKS
---------------
##Drush Steps (recommended):##
1. drush codit-blocks-add {blockname}  - This one command has created the block
   directory in codit_local/blocks/block_bin/ and created the template file and
   _callback.inc.
2. Your new block is now registered as a block.  You can move on down to
   'Your Block is Ready.'

##Manual Steps##
1. Codit: Blocks are created by copying the _a_sample_block directory inside
   codit_local/blocks/block_bin and naming it with the new block name (delta).
   example: cp -r codit_local/blocks/block_bin/_a_sample_block codit_local/blocks/block_bin/cool_block
   You must use underscores not hyphens and keep it shorter than 32 characters.
   Do not precede the name with an underscore, that is used to disable a block.
2. Rename the tpl.php file in the directory to match the block name.
   example: rename _sample_block.tpl.php codit_blocks_cool_block.tpl.php
   Note: This tpl can also be copied *(not moved)* to the theme for overriding.
3. Open the tpl and alter extract($blockdata_a_sample_block); to be
   extract($blockdata_cool_block); then save the file.
4. Flush the cache and your new block 'cool block' will be registered with the
   the system. You can move on down to 'Your Block is Ready.'

##Your Block is Ready##
1. Place your block using Context, Panels or the native Drupal block admin.
   Whichever system you normally use, should be fine.
2. Alter the Block Callback function to pass DATA to the block
   (optional, but recommended)
  a) If your block requires any processing of php, this should be done in the
    _callback function located in  _callback.inc in the block directory.
    Anything $_callback() returns will be passed to the tpl (pass multiple
    items in an associative array).
  b) Open codit_local/blocks/block_bin/{block_name}/_callback.inc
  c) Process any data needed for your block. This function will have access to
    the current entity ($o_entity) for the page that the block is on (node or
    term) and be able to alter it by reference.
  d) Best practice is that this should be data, not markup.  Any markup should
    be done on the tpl.   Take this into consideration as you structure the
    items you are returning from the function.
3. (optional) Address any caching needed in $_cache_id() in _callback.inc
  a) Open codit_local/blocks/block_bin/{block_name}/_callback.inc
  b) Alter the function $_cache_id() to return a unique cache ID. Code hints are
    in the function to get you started.
  c) If the block needs its cache to expire, the duration of how long before it
    expires can be set by appending the duration in seconds in a pipe attached
    to the cache_id.
    Example:  For a block to be cached with the id of 'this_is_my_block'  and
    cached for an hour, have the $_cache return 'this_is_my_block|3600'
  d) To disable Codit: Block caching temporarily, use 
    ?{your debug slug}=blocks-no-cache on the url and it will bypass caching.
4. (optional) Address any access restrictions in $_access() in _callback.inc
  a) Open codit_local/blocks/block_bin/{block_name}/_callback.inc
  b) Alter the function $_access() to do any role, permission or user processing
     to return TRUE when it should be visible and FALSE when it should not.
     Code hints are in the function to get you started.
5. Edit your tpl to format the output of the content of your block.


Drush Commands
--------------
* drush codit-blocks-add {blockname} --enable=FALSE
  Creates the block directory, callback file and tpl in codit_local/blocks/bloc_bin.
  and flushes the block cache to register the new block.  The optional enabled
  flag can be set to false to start the block in the disabled state.
* drush codit-blocks-disable {blockname}
  Disables the block if it is enabled and flushes the block cache to de-register
  the block.
* drush codit-blocks-enable {blockname}
  If the block is currently disabled, it will enable it and flush the block
  cache to register it.
* drush codit-blocks-cache-clear-block-list
  Clears the block cache to register or de-register blocks.
* drush codit-blocks-list
  Clears the block list cache and lists the Codit: Blocks that are enabled.
* drush codit-blocks-cache-flush
  Clears the cached data for all Codit: Blocks.
* drush codit-blocks-local-init
  Sets up the blocks directory within Codit local if it does not exist.  This is
  not needed if you use drush to add the first block.


FAQ
---
Q: A block is misbehaving, how do I remove it temporarily?
 : A: You have several options.  The easiest is to add an underscore to the
   front of the directory name.  The underscore tells Codit: Blocks to ignore
   it. The other option would be to remove it from output in Context, Panels or
   the block admin.

Q: I created a block but it is not showing up.  How should I debug?
 : A: There are several reasons why a block would not showing up:

  1. Check the watchdog log.  Most errors will appear there.
  2. The name of the tpl file and the name of the block directory do not match.
    example: Directory name 'block_foo' needs to contain a tpl named
      'block_foo.tpl.php'
  3. The $_access() callback in _callback.inc is returning FALSE.
  4. The tpl is not outputting anything.
  5. The first time you loaded the block, nothing was output and that is now
     cached even though you changed the code.  Flush cache.
  6. It is not properly placed using Context, Panels, or block admin.


Q: I moved the tpl to the theme and the block is not showing up.  What is wrong?
 : A: The tpl for a block must reside in its directory inside
  codit_local/blocks/block_bin/  You can COPY the tpl to your theme to override
  it, but do not MOVE it to your theme.

Q: How can I change the displayed titles of blocks (optional)?
 : A: This is easy to set across the entire site:

  1. Open codit_local/blocks/config_blocks.inc
  2. Add an entry to the $_codit_blocks_block_title_override array with the
    block delta as the key and the actual title you want to be displayed.
    Reminder: When you enable this module, it shows all block titles by default.
    However by setting
    $_codit_blocks_block_title_override['show-block-titles'] = FALSE
    you can force it to not show any but the ones you have overridden. Many
    times it is more convenient to disable them all so that blocks roll-up when
    they are empty, so it is best to output a title from the tpl rather than
    using Drupal's block title system.
  3. To accommodate allowing block titles on blocks that have dynamic hashed
    block deltas. (example, the facet blocks built by apacheSOLR. You actually
    want those titles to show, rather than be hidden. To allow a given module
    to have all its blocks have titles, add a line like this to the
    $_codit_blocks_block_title_override array 'module_name' => 'all',

Q: Will this work with core Drupal Block caching enabled?
 : A: Yes, Codit Blocks will function with core drupal block caching enabled,
  however, Codit: Blocks caching is much more granular and works best if core
   block caching is disabled.

ROADMAP
-------
* Conversion to Drupal 8 such that migrating a block from D7 to D8 will be as
  simple as copy and paste of the block directory. *(Assuming nothing used in
  that block is specific to D7.)*



BONUS FEATURES
--------------
The following module is not required, but if you have it enabled it will
improve the experience:

 * Markdown - When the Markdown filter is enabled, display of the module help
   for any of the Codit modules and submodules will be rendered with markdown.

Similar Block Projects and how they differ
-------------------------------------------
We have a choice of 4 modules to create custom blocks for a site:

* Codit: Blocks
* Block Module
* <a href="https://www.drupal.org/project/custompage" rel="nofollow">Custom Page module</a>
* <a href="https://www.drupal.org/project/boxes" rel="nofollow">Boxes module</a>

See: <a href="http://web-dev.wirt.us/info/drupal-7/comparison-drupal-7-block-building-modules"
rel="nofollow">Comparison of Drupal 7 block making methods.</a>
Codit: Blocks is more robust in many ways, offering developers who code a lot
more control over their blocks.

* **Code** - 100% code based. The only admin is whatever tool you use to place
  them (block admin, Context, Panels)
* **Caching** -  Simple yet highly capable caching ranging from sitewide to per
  user or per page, or even per-user-per-subpath on a block by block basis.
* **Individual Block User Access** - Easy to define access based on role, user
  or any other custom criteria.
* **Editing Blocks** - Only someone with developer access can modify these
  blocks.
* **Heavy PHP Processing** -  resides safe from collisions in an anonymous
  callback function that passes items to the template for the block.
* **Output** -  Use a block specific tpl.php for each block. It can be copied to
   the theme if needed to have different output on sites with multiple themes.


MAINTAINERS
-----------
* Steve Wirt (<a href="https://drupal.org/user/138230" target="_blank">swirt</a>)
