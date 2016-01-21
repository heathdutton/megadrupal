
 THEME BLOCKS
 ------------

This module allows themes to define their own blocks. 

You probably only need this if a theme has asked you to install it
or if you are a themer and you want to declare some blocks without
creating your own module.

Why? The code to define a simple block is easy to create, but also 
very easily abstracted. Why not provide it as a simple feature for 
themers to use, without having to create a custom module?

 USAGE
 -----

1) Download to sites/all/modules (or wherever you have decided to 
store your module files).

2) Activate the module at admin/build/modules.

3) Any blocks defined in your theme are now available on the block
admin page at admin/build/blocks.

 USAGE FOR THEMERS
 -----------------

To define a block you just add a line to your theme.info file:

theme_blocks[BLOCKNAME][template] = TEMPLATE

BLOCKNAME is a unique identifier for the block, and TEMPLATE is the
name of the file in your theme folder to use for the block. For
example, to create a block for an advert I have stored in my theme
in a file called advertisment.tpl.php I would add the following to
theme.info:

theme_blocks[myadvert][template] = advertisment

Notice that the .tpl.php is not required on the end of the template
name. The block name is a unique name for the block, and should
be lowercase letters and underscores.

 ADVANCED USAGE
 --------------

You can defined more properties for you blocks. Here is a full 
example of what options are currently implemented:

theme_blocks[sidebar_ad][template] = my-sidebar-ad
theme_blocks[sidebar_ad][title] = Advertisment
theme_blocks[sidebar_ad][info] = My Skyscraper Advert
theme_blocks[sidebar_ad][weight] = 0
theme_blocks[sidebar_ad][status] = 1
theme_blocks[sidebar_ad][region] = right
theme_blocks[sidebar_ad][cache] = BLOCK_CACHE_GLOBAL

As you can see the block name is the same for each property on the
block. You can repeat this code block again to define multiple 
blocks - just make sure they all have a unique block name.

The options you can define:

template = the name of the file with the code for this block
title = a default title for the block, if required
info = the title of the block used on the block admin page
weight = default weight for the block, overriden on block admin page
status = set this to 1 and block is enabled by default, otherwise 0
region = the default region for this block, if enabled
cache = see hook_block() for information on block caching options

If you don't want to put all your block template files in the root
of your theme folder you can put them in a subfolder and then add
the following configuration option to you theme.info file:

theme_blocks_path = FOLDERNAME

Where FOLDERNAME is the subfolder that contains your block template
files. For example, to store all your block templates in a folder
called 'templates' you would add the following line to theme.info:

theme_blocks_path = templates


 CONTACT
 -------

For more information visit the developer's blog:
http://www.darrenmothersele.com

