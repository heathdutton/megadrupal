Assetic SASS support 1.x for Drupal 7.x
---------------------------------------
Assetic SASS adds support of the [SASS language][1] to the main Assetic module
in Drupal.

Installation
------------
Make sure that the Assetic module is correctly working and enable this module.
You also need to have the SASS Ruby gem installed on your system. You can find
instructions for this [here][2]

Usage
-----
To use the SASS language in your theme's stylesheets, add the .scss
or .sass extension to these files. These files must be added to your theme
like you normally would add a stylesheet in Drupal. This module will
automatically pick them up and renders them to valid CSS.

[1]: http://sass-lang.com/
[2]: http://sass-lang.com/download.html
