==INFO==
Using PHP-ref is a debugger for Drupal.


==INSTALL==
Install PHP-ref in libraries folder.
Example:
sites/all/libraries/php-ref

Path for phpref.php should in the example be:
sites/all/libraries/php-ref/phpref.php

==USAGE==
Enable module.

Basic usage from php-ref documentation:

Include the class (not needed if project runs with Composer because it's auto-loaded)
require '/full/path/to/ref.php';

Display info about defined classes
r(get_declared_classes());

Display info about global variables
r($GLOBALS)

To print in text mode you can use the rt() function instead:
rt($var);

To terminate the script after the info is dumped, prepend the bitwise NOT operator:
~r($var);   // html
~rt($var);  // text

Prepending the error control operator (@) will return the information:
$output = @r($var);   // html
$output = @rt($var);  // text

Keyboard shortcuts (javascript must be enabled):
X - collapses / expands all levels

For more options, see: https://github.com/digitalnature/php-ref
