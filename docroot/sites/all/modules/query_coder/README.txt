CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Usage
* Known issues

INTRODUCTION
------------

Current Maintainer: Denis Zakharov
Profile: http://drupal.org/user/2502862

Convert SQL query to Drupal Database abstraction layer code.
Module support only MySQL SQL dialect.

INSTALLATION
------------

1 Download and install
  Libraries API module (http://drupal.org/project/libraries)
2 Download PHP-SQL-Parser (https://code.google.com/p/php-sql-parser/) library
  and unzip it to "sites/all/libraries" folder.
  Full path must be "sites/all/libraries/PHP-SQL-Parser/php-sql-parser.php"
3 Download and install Query coder module.
4 Go to admin/config/development/query_coder.

USAGE
-----

This module create for help developers convert MySql queries into
Drupal Database abstraction layer code during development process.

KNOWN ISSUES
------------

1 INSERT queries must be specified in form
  "INSERT INTO table_name (column1, column2,...) VALUES (value1, value2,...)"
2 Not support "->expression()" conditions for UPDATE queries.
3 All tables and fields in SELECT query must be specified with aliases,
  in other (UPDATE, INSERT, DELETE) queries without aliases
  Example:
  RIGHT: "SELECT * FROM users u"
  NOT RIGHT: "SELECT * FROM users"
4 All combining conditions in SELECT queries must be specified with aliases.
  Example:
  RIGHT: "SELECT COUNT(*) AS user_count FROM users u"
  NOT RIGHT: "SELECT COUNT(*) FROM users u".
