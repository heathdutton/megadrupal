V8 JavaScript Engine
====================

Ensures that the V8 JavaScript Engine (http://code.google.com/p/v8/) is
available to Drupal. You should not need to install this module unless it is
required by another module.


Installation
------------

1. Install the v8js PHP extension by running "pecl install v8js", and adding
   "extension = v8js.so" to your php.ini.
   http://www.php.net/manual/en/book.v8js.php

2. Enable the module at admin/modules

3. Visit admin/help/v8js for a demonstration of the module in use
