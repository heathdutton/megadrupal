Introduction
------------

A system to keep 'structural' entities in code, using Migrate to import them and
inspired by Features for their storage.

This is intended to be used with entities that are treated as part of a site's
structure rather than content. For example, on drupal.org this might be the
taxonomy terms for major versions, '6.x', '7.x' and so on.

Migrations are defined automatically by parcels: these are a module-like folder
with a .info file and a CSV file of exported entities. The CSV file headers are
assumed to match the key schema for the entity being imported.

Specification
-------------

An parcel is defined having a folder and file structure similar
to a module in sites/all/parcels:

- sites/all/parcels/my_import
- sites/all/parcels/my_import/my_import.info
- sites/all/parcels/my_import/my_import.csv

The info file has the following properties:

machine_name = The migration class machine name e.g. MyImport. Appears in the Migrate UI.
description = The human-readable description. Appears in the Migrate UI.
entity = The machine name of the entity type to use.
bundle = The machine name of the entity bundle to use.
; The following are optional:
; define dependent migrations
dependencies[] = MigrationMachineName
; Define one or more mapping default values:
mapping_defaults[pathauto] = 0
; Source key for the MigrateSQLMap. If omitted, the entity primary key is
; used (which means your CSV must have it, though it is not used for import).
; Provided this is unique, it can be anything.
key = name

TODO
----

- look into whether this should be a Features component rather than standalone
- tackle more tricky field types such as files
- allow specifying dependencies in the .info
