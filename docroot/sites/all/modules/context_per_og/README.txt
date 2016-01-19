Context per Organic Group
================================

This module creates one context per Organic Group.

* When a new Organic Group is created, a new context is created with a default
"OG - group node context" condition set for the new group.
* When an Organic Group is deleted, the associated context is also deleted.
* All users are prevented from deleting the automatically-generated context
  by a simple modification to the context "confirm delete" form.

New features on roadmap:

* Provide admin form to disable this behavior per group type
* Add alter hooks to allow modification of automatically created context
* Create a drush command that will automatically generate contexts for existing
  Organic Groups that do not currently have an associated context.
