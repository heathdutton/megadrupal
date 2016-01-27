JpGraph
-------
JpGraph is a PHP library used to create charts, all kinds of charts.
This Drupal module provides a simple loader for this library.

Dependencies:
-------------
 * Library module

How to install:
---------------
 1. Install the JpGraph module as any other module.
 2. Install the JpGraph library:
   * Download it from http://jpgraph.net and put it
     in Drupal's default libraries folder, OR
   * Use the makefile in the module to automatically download it by running:
     drush make --no-core -y --contrib-destination=. path/to/module/jpgraph/jpgraph.make
     from within the sites/all directory.

Usage
-----
You can use the jpgraph_load() function to load any file from jpgraph.
Example:
 * drupal_load();                             // will load only jpgraph.php
 * drupal_load(array('jpgraph_regstat.php')); // will load jpgraph.php
                                                 and jpgraph_regstat.php

Contributors:
-------------
 * Pol Dell'Aiera, author of the Drupal's module.
