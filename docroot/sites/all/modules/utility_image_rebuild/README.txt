Utility-Image-Rebuild-module
============================

Drupal module - Utility Image Rebuild module

Drupal Core: 7.x

CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Dependencies

Introduction
------------

Current Maintainer: Joseph Z (joseph@gotouch.com.au)

This module is a small utility tool.

It can help you rebuild the image(s) information stored in the image field
within certain content types.

The two common cases are:

1. Image(s) were replaced manually on the server, such as cover.jpg from a small
   size to a larger size.
2. Image(s) name(s) or path(s) were updated in the database and were replaced
   manually on the server.
   For example, from public://publication/1.jpg to public://article/01.jpg

In particular, the {file_managed.filesize} and the {file_managed.timestamp}
columns will be incorrect after making these changes and the image dimension
in the field_data_filename table will be incorrect as well.

This module will rebuild said information and then update them in database.

Installation
------------

Installation is quite simple:

Enable the module from admin/modules or use Drush:

drush pm-enable utility_image_rebuild

Dependencies
------------

The Utility Image Rebuild module is dependent on two Drupal 7 core modules:
file
image

You will be prompted to enable these modules if they are not already enabled.
