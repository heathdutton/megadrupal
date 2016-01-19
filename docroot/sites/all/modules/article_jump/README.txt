============================
Article Jump
============================


About this module
--------------------

Article Jump is a Drupal module to add jQuery functiontionality to scroll from
one article to the next via a hotkey. For instance you can scroll down through a
stream of node teasers by pressing "j" several times. Then scroll back to the
previous article by pressing "k".

The jQuery selector is configurable, so if you have a stream of feed items, you
can add a selector of ".content .feed-items" on the module configuration page
and users will be able to navigate that stream by pressing "j" and "k" as well.

The hotkeys are also configurable.

Requirements
-----------------

This module requires the libraries module and Mousetrap.js, a javascript
library.


Installation
---------------

1. Create a libraries directory at: sites/all/libraries/
2. Download and enable http://drupal.org/project/libraries
3. Download Mousetrap.js into sites/all/libraries/moustrap/
https://raw.github.com/ccampbell/mousetrap/master/mousetrap.min.js
4. Download and enable Article Jump.


Configuration
----------------

On the module's configuration page you can configure the jQuery selectors that 
will be used as the article jumping anchors and also the speed of the scroll.

On the configuration page you can also change the hotkeys for scrolling forward 
and back to the next or previous article.

Browser Compatibility
---------------------------------

The functionality has been tested and works in latest versions of Chrome,
Firefox, and Opera. And is even compatible with ye olde IE 8.
