CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers


INTRODUCTION
------------

This module is intended to allow entering all desired translations of a given
content inside of a unique node, instead of having to create a separate node
(with Multilingual Content) or a distinct field version (with Entity
Translation) for each translation.

Multilang introduces the "multi" syntax (look at HELP.md for details): a 
"multi" segment may contain multiple translations of the same text, and will be
rendered as only the current language part.
You may insert "multi" segments anywhere in text fields of nodes, in the body
of a block and in the title of nodes and blocks.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/cfreed/2469725
   You can also usefully consult the HELP.md file, which contains the entire
   documentation (the same as the integrated inline help):
   http://cgit.drupalcode.org/sandbox-cFreed-2469725/tree/HELP.md

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2469725


REQUIREMENTS
------------

This module requires the following modules:
 * Locale (Drupal core)


RECOMMENDED MODULES
-------------------

 * CKEditor (https://www.drupal.org/project/ckeditor)
   WARNING: it also requires the CKEditor Widget plugin to be activated
   (http://ckeditor.com/builder).

Besides that, Multilang is intended to well work with the following modules 
when they are active (look at HELP.md for details):
 * Pathauto (https://www.drupal.org/project/pathauto)
 * Views (https://www.drupal.org/project/views)


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

The module comes with a wide set of configuration options, but they only regard
preferences, so you can directly work with the default settings. See inline help
for more information.

Apart from this, it only depends on other configuration points:
 * for its main job to work, simply define each of the desired languages (in
   admin/config/regional/language) and set detection method to URL (in
   admin/config/regional/language/configure): then Multilang will render
   contents translated depending on the current url language, everywhere it
   founds some multilingual segments.
 * when also using CKEditor, configure each format you want to accept the
   Multilang plugin: then the "multi" syntax will be hidden, and you can enter
   "multi" segments through a set of distincts areas for each language
 * when also using Pathauto, go to [node:multilang-native-title] and turn any [type:token] pattern to [type:mms-native-token] everywhere token may contain
   multilingual segments.


TROUBLESHOOTING
---------------

Nothing registered yet.


FAQ
---

Nothing registered yet.


MAINTAINERS
-----------

Current maintainer:
 * Fred Barboteu (cFreed) - https://www.drupal.org/u/cfreed

This project has been sponsored by:
 * P-interactif - http://www.p-interactif.com
