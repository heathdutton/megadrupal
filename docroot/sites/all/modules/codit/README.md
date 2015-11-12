Codit
=====

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * Submodules
 * FAQ
 * Roadmap
 * Bonus Features
 * Maintainers

INTRODUCTION
------------
This is the base Codit module that contains the contrib (un-alterable) code for
a site.  Consider it a framework or an environment.   It must be paired with
Codit: Local which contains all the local site-by-site customized code and
templates.

The Codit module's **basic premise** is that it is easier for developers to push
code changes than it is to push database changes.  So the entire module is built
to support developers using code rather than admin settings.

The Codit environment is broken into two separate modules so that the base
module, Codit, can reside in sites/all or sites/default (or be used as a
<a href="http://git-scm.com/book/en/Git-Tools-Submodules">git submodule</a>)
and remain unaltered.  Codit: Local on the other hand would reside in the
directory of the site and is intended to be altered specifically for each site.

FEATURES
--------
  * Developer Friendly Code Recepticles
    - An external javascript file (codit_local.js) that is separate from the
      theme, ready for your custom JS.
    - PHP functions added to codit_function_definitions.inc will be available
      throughout Drupal.
    - codit Local Hooks - register any site specific hooks in codit_local.module
      without the need to build a new module just to register a few site
      specific hooks.

  * Friendly Functions for Coders - There are a few functions designed to help
    coders safely do their work.
    - Template Tracers - output html comments, if the user has the proper
      permissions, that reveal where the template begins, ends and resides.
      This is especially useful for multi-sites and multi-dev sites.
      http://web-dev.wirt.us/info/drupal-7-php/drupal-template-tracers
    - Debug Helper - Set up a specific query parameter to output any array,
      object or value you need if the user has the proper permissions.
      Extremely useful for debugging something on a live production server.
      http://web-dev.wirt.us/info/drupal-7/codit-debug-output-from-get
    - PHP Memory and Error Log Writer - Set a message in the php error log to
      keep track of memory or other messages.  This is very useful when running
      heavy processes that may need to be killed off if they prove to be too
      intensive.  It is also useful for keeping tabs on processes that run on
      cron when no information is sent to the screen for a human to see.
    - Column Output - Easily output lists or divs that break a long collection
      of items into evenly ballanced columns that can read down, or across.
      Classes support css treatment for specific columns or rows.
      (example: a visual gap at row 6)
    - First Middle and Last Classes - easy output of these classes for any set
      of items that can be looped through.
      http://web-dev.wirt.us/info/drupal-7/first-and-last-class-list
    - Boolean Sanitization and Transformation - Easily convert a numeric boolean
      to a string boolean and many other transformations.
      http://web-dev.wirt.us/info/code-snippets-php/sanitize-boolean-values-and-control-their-format



REQUIREMENTS
------------
Dependency: Codit: Local


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.


CONFIGURATION
-------------
 * Permissions /admin/people/permissions#module-codit
   - Permissions need to be granted to see debug output
 * Settings /admin/config/codit
    - The debug slug must be set for security reasons.  Do not use "debug".


SUPPORTING MODULES
------------------

Currently Codit has the following supporting modules:

  * Codit: Blocks - Allows the easy yet powerful creation of blocks, block tpls
    and block callback functions to do the heavy lifting and keep the heavy php
    logic out of the tpl.  It is a code based alternative to the admin based
    custom page module https://drupal.org/project/custompage and default Drupal
    block admin.
    Documentation : https://drupal.org/project/codit_blocks


FAQ
---
Q: How do I easily create a new block?
 : A: View the README.md inside codit_blocks which is also visible in
   admin/help/codit_blocks.

Q: Where do I find the developer functions for my use?
 : A: They are all fully documented in codit_functions_for_coders.inc and in
      codit_output_func_for_coders.inc


ROADMAP
-------
The following modules are slated for release soon:

 * Codit: Responsive - allows for moving blocks on, off, and around
   the page based on screen state.
 * Codit: Crons - a framework to create custom cron tasks and timing.


BONUS FEATURES
--------------
The following modules are not required, but if you have them enabled they will
improve the experience:

  * Devel - When Devel is enabled, output from codit_debug will use kpr() rather
    than print_r() to display arrays or objects with the help of krumo.
  * Markdown - When the Markdown filter is enabled, display of the module help
    for any of the Codit modules and submodules will be rendered with markdown.

MAINTAINERS
-----------
* Steve Wirt (swirt) - https://drupal.org/user/138230
