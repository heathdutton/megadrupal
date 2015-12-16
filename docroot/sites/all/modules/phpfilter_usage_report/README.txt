CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Usage
 * API
 
INTRODUCTION
------------
This simple module provides a report showing where (in which entities) the PHP code format (from the core PHP filter module) is used.
It also shows which blocks are using the php_code format.

By default this searches the revision table for each text field, as well as the <em>block_custom</em> table's <em>format</em> field.

INSTALLATION
------------
Install like you would any Drupal 7 module.

There are permissions for this module so you can extend viewing rights to specific roles.

USAGE
-----
View the report at <strong>Admin » Reports » PHP filter usage</strong>.
