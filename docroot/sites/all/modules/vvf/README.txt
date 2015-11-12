This module provides a "Views Variable Field" handler to display a system
variable as a field in a view.

VVF was designed in response to the unsafe practice of using the PHP filter in
Views to add and use system variables.

VVF supports Drupal's standard variables system as well modules that make use
of the Variable module (http://drupal.org/project/variable) for their variables.


Installation
------------

1. Follow the normal Drupal module installation procedures.


Usage
------------
This module provides a new Global field type for Views which enables site
builders to add any system variable as a field in a view.

Usage is straightforward. Select "Global: System variable" from the list of
available fields and then enter the name of the variable to display.

All standard Views field manipulations are supported, such as rewriting the
output value as well as usage in the "No results behavior" via replacement
patterns.


Authors/Credits
---------------

* Author: [plopesc](http://drupal.org/user/282415)
* Development sponsored by [Bluespark](http://bluespark.com).
