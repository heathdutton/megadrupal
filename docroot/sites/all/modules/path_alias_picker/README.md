PATH ALIAS PICKER
=================


CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

Path Alias Picker allows you to search through taxonomy terms and menus and pick
your path alias through them.

Note that the module isn't very clever (yet). It's only able to append parts
to the existing URL.

REQUIREMENTS
------------

This module requires the following modules:

 * Pathauto (https://drupal.org/project/pathauto)


RECOMMENDED MODULES
-------------------

 * Libraries (https://www.drupal.org/project/libraries)
 * jQuery Chosen library (https://github.com/harvesthq/chosen/releases/download/v1.2.0/chosen_v1.2.0.zip)


INSTALLATION
------------

Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

If the jQuery Chosen library is available you'll be able to search through
your taxonomy terms and menu items from a fancy select box, instead of just
picking them out from a default select element.

When you have downloaded and enabled the Libraries modules, download the Chosen
library, extract it to 'sites/all/libraries/chosen/' folder and Path Alias
Picker should be able to find it automatically.


CONFIGURATION
-------------

To configure which vocabularies and menus that should be available, to select
your path from, go to http://example.com/admin/config/search/path/settings. At
the bottom of the page, expand "Path Alias Picker". Select the vocabularies and
menus that you need, and submit the form.

You can now got to any node and where you normally set your path manually, you
should now be able to see one select element for each selected vocabulary or
menu. Make sure that the "Generate automatic URL alias " checkbox is not set, as
the select elements will be inactive if it is.


MAINTAINERS
-----------

Current maintainers:
 * Tommy Lynge Jørgensen (TLyngeJ) - https://www.drupal.org/u/tlyngej
