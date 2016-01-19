CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Requirements
 * Known Problems
 * FAQ

INTRODUCTION
------------

The Commerce XLS Import module is a Drupal Commerce module that allows the
import of Drupal Commerce Products from an XLS file.

This module is meant to be used on sites built using Commerce Kickstart.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/cbanman/2504005


 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2504005


FEATURES
--------

 * Import Commerce products from XLS
 * Import product images
 * Generate template XLS for each product type
 * Exports XLS containing information on successful and unsuccessful product
   imports



REQUIREMENTS
------------

This module was built and tested to work with Commerce Kickstart and may not
work properly with Drupal Commerce depending on your set-up.

 - Modules -

 * PHPExcel (https://www.drupal.org/project/phpexcel)
 * Background Process (https://www.drupal.org/project/background_process)
 * Commerce Product (https://www.drupal.org/project/commerce)

 - Libraries -

 * PHPExcel 1.7.8 or higher (https://phpexcel.codeplex.com/releases/view/96183)

 PHPExcel library can be extracted in any libraries folder you want
 (sites/*/libraries)


KNOWN PROBLEMS
______________

 * Can't set variation titles (https://www.drupal.org/node/2509178)


FAQ
---

 * How do I upload product images?

    In order to upload product images they must be in a .zip file place in
    /sites/default/files/import_images/ directory of the Drupal install
    (public://import_images/). Also make sure that the permissions of the image
    files allow them to be readable by all.
