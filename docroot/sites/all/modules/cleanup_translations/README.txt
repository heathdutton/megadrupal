
INTRODUCTION
------------

The database tables used for string translations are ever-growing as entries
are never removed. Old strings from removed modules, corrected typos in strings
or obsolete strings bloat the tables. Translators do not know which string is
actually used when they are using the translation UI. Also the bloated tables
slow down the translation lookup queries.

This module syncs the strings in the database with those defined in code and
allows to remove outdated strings from the database.


INSTALLATION
------------

This module depends the translation template extractor (potx) module. You also
have to install that module: https://drupal.org/project/potx.


USAGE
-----

The module adds a tab 'Clean up' to the 'Translate interface' pages and can be
accessed using admin/config/regional/translate/clean.
Before removing any strings the user is provided with an overview of all
strings that will be removed.
