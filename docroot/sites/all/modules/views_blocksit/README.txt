About
======

This module allows you to use the Blocksit JS library as a Views style plugin.

About Blocksit
---------------

Library available at:
http://www.inwebson.com/jquery/blocksit-js-dynamic-grid-layout-jquery-plugin/

From the website:
"BlocksIt.js is a jQuery plugin for creating dynamic grid layout like Pinterest.
It allows join of 2 or more blocks into a big block element."

Installation
=============

Dependencies
------------

- [Libraries API 2.x](http://drupal.org/project/libraries)
- [Views API 3.x] (http://drupal.org/project/views)

Setup
-----

1. Download the Blocksit library from:
   http://www.inwebson.com/jquery/blocksit-js-dynamic-grid-layout-jquery-plugin/

2. Create a folder called "blocksit" in the libraries directory.
3. Put either blocksit.js or blocksit.min.js into the blocksit
   libraries directory.
4. In the libraries/blocksit folder, there should be a file called blocksit.js
   or blocksit.min.js. blocksit.min.js will automatically be used if both
    are present.
5. Ensure you have a valid path similar to this one for all files
    - Ex: sites/all/libraries/blocksit/blocksit.min.js

That's all!


Usage
======

This module can be used by selecting the "Blocksit" option from Views format
option, after enabling the module.
The corresponding settings pane contains all of the settings for this module.

Break Points
-------------

You can define breakpoints for when the number of columns should change.
The syntax for breakpoints is as follows:

[container_width, number_of_cols] [next_break_point, number_of_cols]

Note that break points should be ordered with larger widths first. For example:

[1200, 5] [1000, 4] [800, 3] [700, 4]

What the above is saying is that when the container width is 1200px or less,
show 5 columns,
at 1000 or less show 4 columns, etc. The columns can increase, such as in the
last break point or decrease as needed. When a breakpoint is reached,
the columns will rearrange using a CSS3 transition if supported.
