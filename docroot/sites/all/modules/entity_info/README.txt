Entity Info Storage
===================

Problem
-------

Entity information is not stored persistently anywhere. The information is
parsed from hooks and temporary cached. It's true that Drupal stores entity info
in the field_bundle_settings_ENTITYTYPE__BUNDLENAME variables but those
variables are only inserted/changed via UI and for this reason we cannot rely on
them.

This will make almost impossible to upgrade to Drupal 8 using the new "migration
in core" system. All pieces, like view modes or others, that are stored along
with entity info, cannot be evaluated as sources for D7 to D8 migration.

Solution
--------

The module stores the entity information into system variables. The migration
manager should visit the admin/config/system/entity_info on D7 site prior
running the migration. In this way the migration will be provided with
reasonable source data.
