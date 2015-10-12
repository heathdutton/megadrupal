The Dindent module is dedicated to developers who suffer from OCD and cannot
bare reading template engine produced output.

Using the Dindent parser written by Gajus Kuizinas (https://github.com/gajus),
this module allows you to beautify your Drupal HTML without lifting a finger.

Dindent was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark
- http://twitter.com/Decipher



Installation
--------------------------------------------------------------------------------

* Download the Parser.php file from Github:
  https://raw.githubusercontent.com/gajus/dindent/master/src/Parser.php

* Place the Parser.php file into a 'dindent' directory in your libraries
  directory so you have something like 'sites/all/libraries/dindent/Parser.php'.

* Install as usual:
  http://drupal.org/documentation/install/modules-themes/modules-7



Usage
--------------------------------------------------------------------------------

The module has no configuration, all you need to do is ensure it is installed
correctly and enabled, and any cached pages will be automatically processed.



Makefile entries
--------------------------------------------------------------------------------

For easy downloading of Dindent and it's required/recommended modules and/or
libraries, you can use the following entries in your makefile:


  projects[dindent][subdir] = contrib
  projects[dindent][version] = 1.2

  libraries[dindent][download][type] = get
  libraries[dindent][download][url] = https://raw.githubusercontent.com/gajus/dindent/master/src/Parser.php



Roadmap
--------------------------------------------------------------------------------

- Add more granular controls for when module is active/inactive.
- Allow for use on arbitrary blocks of HTML as a Filter or Rules action.
