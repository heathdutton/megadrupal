Remy Sharp's HTML5 Enabling Script
----------------------------------

Adds a small amount of code to the page so that HTML5 output will work
correctly in IE.  This is a 100% copy of Remy Sharp's HTML taken from his 2009
blog post [1].

Installation
------------
Install the module in the usual way:
* Add it to sites/all/modules or sites/all/modules/contrib.
* Enable it via Drush or the admin/modules page.

There are *no* configuration options, *no* permissions, nothing. :)

Once the module is enabled the following will be added to the top of ever page
after the other scripts are loaded:
<!--[if lt IE 9]>
<script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
<![endif]-->

That's it!

Rationale
---------
This module was built as older versions of Internet Explorer do not support the
HTML5 tags that are now in use by many themes, e.g. Omega [2], Boron [3], 
Panels_960gs [4], etc.

Author
------
Created by Damien McKenna [5] for Bluespark Labs [6] based entirely on Remy
Sharp's code [1], so all thanks go to Remy for putting the script together.


1: http://remysharp.com/2009/01/07/html5-enabling-script/
2: http://drupal.org/project/omega
3: http://drupal.org/project/boron
4: http://drupal.org/project/panels_960gs
5: http://drupal.org/user/108450
6: http://www.bluesparklabs.com/
