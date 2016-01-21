CONTENTS OF THIS FILE
---------------------

 * About verge.js
 * About verge_lib
 * Dependencies
 * Installation
 * Configuration
 * Integration
 * verge methods
 * Links


ABOUT VERGE.JS
--------------

"verge is a compact set of cross-browser viewport utilities packed into 
an opensource JavaScript module. It provides methods to measure viewport 
dimensions and/or to detect if a DOM element is in the viewport."
  (taken from http://verge.airve.com/)

As one can see at http://ryanve.com/lab/dimensions/ the verge approach 
offers the most precices results for viewport dimensions (browser window 
width and height) and is also the winner in cross-browser compatibility. 


ABOUT VERGE_LIB
---------------

This module just adds the verge.js library into drupal's library array; 
it doesn't have a user interface.

It's main purpose is to be used as standard for any responsive/adaptive 
module working with viewport dimensions. Actually many modules are using 
their own scripts therefore; so measurement results vary a lot.


DEPENDENCIES
------------

(1) libraries (2.x):
    https://drupal.org/project/libraries

(2) verge.js library


INSTALLATION
------------

(1) Place the module in your standard modules location (usually
    'sites/all/modules').

(2) Place the library in your standard libraries location (usually 
    'sites/all/libraries')

    Either download the single files from the authors homepage or 
    download the git master branch.

    At least your verge.js library structure should look like:

    path/to/libraries/
                      verge/
                            verge.js
                            verge.min.js

(3) Enable the module:
    'admin/modules'


CONFIGURATION
-------------

This module does not offer any configurable options.


INTEGRATION
-----------

To make use of the library within custom code keep in mind:

(0) *optional*
    The library's installation can be checked:

    if (verge_lib_installed()) {
      [CUSTOM_CODE]
    }

(1) The library needs to be called within PHP:

    if (verge_lib_loaded()) {
      [CUSTOM_CODE]
    }

(2) jQuery scripts need to be extended when verge shall be used:

  jQuery.extend(verge);

Then a script can make use of all verge methods.


VERGE METHODS
-------------

@return number
  $.viewportW() // Get the viewport width in pixels.
  $.viewportH() // Get the viewport height in pixels.
@return boolean
  $.inViewport(elem) // true if elem is in the viewport
  $.inViewport(elem, 100)  // true if elem is in the viewport or 100px near it
  $.inViewport(elem, -100) // true if elem is in the viewport at least 100px
  $.inX(elem) // true if elem is in same x-axis as the viewport
  $.inY(elem) // true if elem is in same y-axis as the viewport
    On pages that never scroll horizontally, $.inX always returns true!
    On pages that never scroll vertically, $.inY always returns true!
@return object
  $.rectangle(elem) // Get coordinates relative to the viewport.
  $.rectangle(elem, 100)  // Get coordinates, with 100px cushion.
  $.rectangle(elem, -100) // Get coordinates, with -100px cushion.


LINKS
-----
verge.js website              - http://verge.airve.com/
verge @ github                - https://github.com/ryanve/verge
issues @ github               - https://github.com/ryanve/verge/issues
Ryan Van Etten (verge author) - http://ryanvanetten.com/
R2-D8 (drupal maintainer)     - https://drupal.org/user/420324/
project @ drupal              - https://drupal.org/project/verge_lib/
issues @ drupal               - https://drupal.org/project/issues/verge_lib/
