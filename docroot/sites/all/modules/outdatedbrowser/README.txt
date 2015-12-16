Outdated Browser module
------------------------------

DESCRIPTION
-----------

This module integrates the Outdated Browser library [1] in Drupal. It detects
outdated browsers and advises users to upgrade to a new version - in a very
pretty looking way.

The library ships with various languages. Its look and feel is configurable,
and the targeting browser can be configured either specifying a CSS property or
an Internet Explorer version.

More info at: http://outdatedbrowser.com

REQUIREMENTS
------------
This module requires the following modules:
 * Libraries API (https://drupal.org/project/libraries)


INSTALLATION
------------
1. Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
2. Download and unpack the contents of the Outdated browser plugin in
   "sites/all/libraries".
   Make sure the path to the plugin file becomes:
   "sites/all/libraries/outdatedbrowser/outdatedbrowser/outdatedbrowser.min.js"
   Link: https://github.com/burocratik/outdated-browser
   ATTENTION: The parent directory of the Github download does have a hyphen in
   its name! So make sure to remove the hyphen to have a valid directory name!


CREDITS
-------

The Outdated Browser module was originally developed and is currently
maintained by Mag. Andreas Mayr [2].

All initial development was sponsored by agoraDesign KG [3].


CONTACT
------------------------------------------------------------------------------

The best way to contact the authors is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  https://www.drupal.org/project/issues/2369737


References
------------------------------------------------------------------------------
1: https://github.com/burocratik/outdated-browser
2: https://www.drupal.org/u/agoradesign
3: http://www.agoradesign.at
