INTRODUCTION
------------
Salsify enables vendors, merchants, and sales staff to manage and exchange
product information (descriptions, features, images, specs, data sheets, videos,
etc.) across the retail supply chain. Founded in 2012 by e-commerce industry
veterans and backed by Matrix and North Bridge, Salsify is already used by
dozens of global manufacturers to increase their brands’ product distribution
and by retailers to increase product range.

The Salsify module supports one way syncing from Salsify to a Drupal site.
 * For a full description of the module, visit the project page:
  https://drupal.org/sandbox/darrenoh/salsify
 * To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/salsify

REQUIREMENTS
------------
A Salsify account is required to use this module. Free trials are available
(http://www.salsify.com/product-tour).

This module requires the following modules:
 * Migrate (https://drupal.org/project/migrate)
 * Drupal Commerce (https://drupal.org/project/commerce)

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.
 * Apply the following patches:
   * Fix for revision value in Migrate Extras (https://drupal.org/node/1951904)
   * Fix for missing subfields in Migrate UI (https://drupal.org/node/2261227)
 * If your database performs case-sensitive string matching, apply a patch to
  fix Drupal's code registry (https://drupal.org/node/2170453)
 * If you need to import product bundles, install Commerce Product Bundle and
  apply patch to enable bundles to use a single product reference field
  (https://drupal.org/node/1489548).

CONFIGURATION
-------------
Create a group of migrations or to add migrations to an existing group in
Administration » Content » Migrate » Import from Salsify.

DEVELOPER REFERENCES
--------------------
 * Channel export API
  (http://help.salsify.com/knowledge_base/topics/channel-exports)
 * Import API (http://help.salsify.com/knowledge_base/topics/import-api)
 * Salsify JSON Import Format
  (http://help.salsify.com/knowledge_base/topics/salsify-json-import-format)
 * Product CRUD API
  (http://help.salsify.com/knowledge_base/topics/product-crud-api)

ROAD MAP
--------
The current version of the Salsify module supports one way syncing of product
data from Salsify to Drupal. Two-way syncing is planned for the next version.
 * Phase 2
  (https://drupal.org/project/issues/search/salsify?issue_tags=7.x-1.1+blocker)

MAINTAINERS
-----------
The Salsify module is a contribution to the community by Double Prime, Inc. with
cooperation from the creators of Salsify.
