Strip 4-byte UTF8
-----------------------------------------------

This module helps in preventing PDO exceptions caused by MySQL general error of Incorrect string value, Enabling this module will have your site reject overly long 2 byte sequences, as well as characters above U+10000, and reject overly long 3 byte sequences and UTF-16.

This is a fix for a long time issue of UTF-8: support 4 bytes characters in MySQL
https://www.drupal.org/node/1314214

The original issue was concerned with data being lost or corrupted by using character sets for MySQL which are not properly supported.

Drupal will always instruct MySQL to create UTF-8 tables, but by default, UTF-8 in MySQL doesn't cover the entire UTF-8
https://dev.mysql.com/doc/refman/5.5/en/charset-unicode-utf8.html

The error log message we may get is as the following 

PDOException: SQLSTATE[HY000]: General error: 1366 Incorrect string value: '\xF0\x9F\x94\xB4\x0D\x0A...'

This is a MySQL issue. which errors come when we do have some weird characters.
