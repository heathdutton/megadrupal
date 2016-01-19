CONTENTS OF THIS FILE
---------------------

 * About verge_lib_measure
 * Dependencies
 * Installation
 * Configuration
 * Integration


ABOUT VERGE_LIB_MEASURE
-----------------------

This small module stores the actual viewport dimensions, measured with 
verge.js, into a cookie. Any responsive module/theme working with 
breakpoints and/or media queries can easily make use of the results.

Additionally the results can be displayed in a jQuery driven div. 
Therefore the module offers role permissions per theme and the option 
to disable the integrated css.


DEPENDENCIES
------------

- verge_lib
  https://drupal.org/project/verge_lib


INSTALLATION
------------

(1) Place the module in your standard modules location (usually
    'sites/all/modules').

(2) Enable the module:
    'admin/modules'


CONFIGURATION
-------------

(1) Set up role permissions to your needs:
    'admin/people/permissions#module-verge_lib_measure'

(2) Check configuration options:
    'admin/config/media/verge-lib-measure'


INTEGRATION
-----------

To make use of the measured viewport dimensions within custom scripts 
simply call cookie values: 

@return number
  $.cookie("verge_width")   // Get the viewport width in pixels.
  $.cookie("verge_height")  // Get the viewport height in pixels.
