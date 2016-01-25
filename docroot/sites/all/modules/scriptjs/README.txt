Integration with $script.js - Async JavaScript loader and dependency manager.

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installation
 * Authors
 * Sponsors

INTRODUCTION
------------

This takes some of the ideas behind the HeadJS module which unfortunately is somewhat discontinued
(barring a new co-maintainer) due to the fact that the head.js javascript library seems to have been abandoned and
applies them to the very excellent $script.js library by Dustin Diaz, which seems like a much more robust script
loader implementation.

Let's see how it plays with Drupal! :)


INSTALLATION
------------

 * Extract the module in your sites/all/modules or sites/all/modules/contrib directory and enable it
 * Download the $script.js library and put it in your sites/all/libraries directory so that it looks like this:
    
    sites/all/libraries/scriptjs/dist/script.js

 * If you use Drush, you can alternatively download the $script.js library directly using:
  
    drush scriptjs-download


AUTHORS
-------

* Alex Weber (alexweber) - http://drupal.org/user/850856 & http://www.alexweber.com.br


SPONSORS
--------

This project is made possible by Webdrop (http://webdrop.net.br)