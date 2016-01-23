Codit Local
===========

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * FAQ
 * Bonus Features
 * Maintainers

INTRODUCTION
------------
This is local module that contains the site-specific code for
a site. It must be paired with Codit which contains all the contrib code.


The Codit environment is broken into two separate modules so that the base
module, Codit, can reside in sites/all or sites/default (or be used as a
<a href="http://git-scm.com/book/en/Git-Tools-Submodules">git submodule</a>)
and remain unaltered.  Codit: Local on the other hand would reside in the
directory of the site and is intended to be altered specifically for each site.

FEATURES
--------
  * Developer Friendly Code Recepticles for local customization.
    - An external javascript file (codit_local.js) that is separate from the
      theme, ready for your custom JS.
    - PHP functions added to codit_function_definitions.inc will be available
      throughout Drupal.
    - codit Local Hooks - register any site specific hooks in codit_local.module
      without the need to build a new module just to register a few site
      specific hooks.



REQUIREMENTS
------------
Dependency: None, although very little happens without the Codit module enabled.


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.


CONFIGURATION
-------------
 * There is no configuration for this module, and no permissions.


FAQ
---
Q: If I want to add a few Drupal hooks that are specific to this site, where
   should I put them?
 : A: Add them to codit_local.module then flush cache.

Q: If I want to add a couple of custom php functions that I want to be available
   to tpls or other places in the site, where should I add them?
 : A: Add them to codit_local_function_definitions.inc and prefix them with
   codit_local to avoid collisions.

Q: If I want to add some javascript that is specific to this site, where should
   I put it?
 : A: Add it to codit_local.js and then flush cache.

BONUS FEATURES
--------------
The following module is not required, but if you have it enabled it will
improve the experience:

 * Markdown - When the Markdown filter is enabled, display of the module help
   for any of the Codit modules and submodules will be rendered with markdown.

MAINTAINERS
-----------
* Steve Wirt (swirt) - https://drupal.org/user/138230
