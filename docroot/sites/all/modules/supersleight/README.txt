SuperSleight module:
--------------------
Maintainers:
  Rob Montero (dgo.to/@rmontero / http://drupal.org/user/191552)
Requires - Drupal 7
License - GPL (see LICENSE)

Overview:
--------
SuperSleight adds a number of new and useful features that have come from the 
day-to-day needs of working with PNGs.

- Works with both inline and background images, replacing the need for both 
  sleight and bgsleight.
- Will automatically apply position: relative to links and form fields if they 
  don't already have position set. (Can be disabled.)
- Can be run on the entire document, or just a selected part where you know the 
  PNGs are. This is better for performance.
- Detects background images set to no-repeat and sets the scaleMode to crop 
  rather than scale.
- Can be re-applied by any other JavaScript in the page – useful if new content
  has been loaded by an Ajax request.

There are two flavors of this script:

Original: http://24ways.org/code/supersleight-transparent-png-in-ie6/supersleight.zip
jQuery: http://allinthehead.com/code/sleight/supersleight.plugin.js

Features:
---------

* Drush command to download and install the SuperSleight plugin to
  sites/all/libraries

Installation:
------------
1. Download the Supersleight plugin in "sites/all/libraries/jquery.supersleight/".
   Drush users can use the commands:
   "drush supersleight-jquery" for the jQuery version or
   "drush supersleight-plugin" for the plain js.
   Notice the module will automatically use one or the other.
2. Download and unpack the SuperSleight module directory in your modules folder
   (this will usually be "sites/all/modules/").
3. Go to "Administer" -> "Modules" and enable the module.

Drush:
------
A Drush command is provides for easy installation of the DOMWindow
plugin itself.

% drush supersleight-jquery

The command will download the plugin and unpack it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not
recommended unless you know what you are doing.

Roadmap:
--------


Change Log:
----------
02/28/2012 rmontero Initial commit.

Last updated:
-------------
02/28/2012