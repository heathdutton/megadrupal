CONTENTS OF THIS FILE
---------------------

* Introduction
* Usage

INTRODUCTION
------------

Current maintainer: Oliver Davies (http://drupal.org/user/381388).

This module creates a customised, token-enabled block that dynamically displays
a copyright statement based on a user-defined start year and the current year.

USAGE
-----

* Download and enable the module.
* Go to admin/structure/block/manage/copyright_block/copyright_block/configure.
* Enter a start year.
* Enter some statement text. An additional token, [copyright_statement:dates],
  is created as part of the module which uses the entered start year and
  combines it with the current year, taken from the server. You can also use
  any other tokens within this text.
* Place the block into a region.
* Save the block.
