Assetic Compass support 1.x for Drupal 7.x
------------------------------------------
Assetic Compass adds support of [Compass][1] to the main Assetic module
in Drupal.

Installation
------------
Make sure that the Assetic module is correctly working and enable this module.
You also need to have the Compass Ruby gem installed on your system. You can
find instructions for this [here][2]

Usage
-----
To use the Compass framework in your theme's stylesheets, add the .scss
or .sass extension for these files. These files must be added to your theme
like you normally would add a stylesheet in Drupal. This module will
automatically pick them up and renders them to valid CSS.

You can also add Compass plugins to your stylesheets in similar a way you
normally would do with a require.rb file. Check assetic_compass.api.php for
more information on how doing this.

[1]: http://compass-style.org/
[2]: http://compass-style.org/install/
