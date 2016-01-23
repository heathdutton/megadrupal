INTRODUCTION
--------
Maintainer: Felix Maier
Profile: http://drupal.org/user/264153
Module Project Page: http://drupal.org/sandbox/leschekfm/1286252

Views Quicksand integrates the jQuery Quicksand plugin into Views. Therefore it
provides a views style-plugin, which allows you to alter most settings the
quicksand plugin provides.

IMPORTANT NOTE: This module won't work without a ajax-view and exposed
filters/sort

INSTALLATION
--------
1. Download and extract the module to [yourdrupalroot]/sites/all/modules/
   (final structure will be [yourdrupalroot]/sites/all/modules/views_quicksand/
2. a) Download 'jquery.quicksand.js' from 'http://razorjack.net/quicksand/' and put
   it into [yourdrupalroot]/sites/all/libraries/jquery.quicksand/
   b) Or use 'drush quicksand-plugin'

USAGE
--------
1. Choose the view you want to enable the animation for
2. Determine a html tag and attribute which will identify a view-row as a unique
   item in your view (like the anchor-tag to a node and the href-attribute)
3. Go to the view edit page
4. Change the plugin under Format to "Views Quicksand"
5. Write the previously determined tag and attribute names into the appropriate
   fields on the settings page (use jQuery-selector like syntax)
6. Unless you haven't already: Expose a filter/sort and set your view to use
   ajax
7. Save your view and you should be good to go

TEST MODULE
--------
In the main module a submodule is included to allow you to verify
views_quicksand is working. When enabled the submodule provides a preconfigured
view which should always show a working animation.

JAVASCRIPT HOOKS
--------
There are two custom javascript hooks you can use to attach own enhancement
functions: beforeQuicksandAnimation and afterQuicksandAnimation.
For a demonstration on how to use them have a look at views_quicksand_test.js
BE AWARE that beforeQuicksandAnimation may be fired twice!

Please test the module and provide feedback.
Thanks for using :)
