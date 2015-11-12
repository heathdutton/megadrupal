Field Complete
==============

In some situations you want to mark an entity field to indicate it should be
completed but not enforce its completion at that time. This module extends the
choice between Required and Not required to "desirable" completion.

This module overcomes the problems with the (otherwise excellent) Content
Complete module which cannot cope with field collections or fields that link
to other entities.

-- FEATURES --

* Works with
  + Entity reference
  + Field collections
  + Node and user references
  With these field types it can (optionally) check for completeness within
  any referenced entity.
  
* Works with Matrix field type

* Sub-module provides a block that lists incomplete fields

* Sub-module provides interoperation with Conditional Fields module

* Provides a Rules condition for testing %age field completion

* Marks the field label with a symbol, like the "required" asterisk

* Integrates with the Editable Field module to ensure clean

For a full description of the module, visit the project page:
  http://drupal.org/project/fc

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/fc

-- WHAT IT DOESN'T DO --

It doesn't (yet) provide:

* A %age completion block

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* Then go to admin/config/content/fc/rebuild and run the Rebuild operation for your site.


-- CONFIGURATION --

Field completeness is set-up on a per-field instance basis, which is to say
any field can be specified as being field-complete with various other options
depending on the type of field. A field used with different node types,
bundles or entities has to be set of for completeness individually.

-- CONTACT --

Current maintainers:
* Steve Turnbull (adaddinsane) - http://drupal.org/user/621584

This project has been sponsored by:
* Digital Life Sciences
  http://www.digitallifesciences.co.uk/
