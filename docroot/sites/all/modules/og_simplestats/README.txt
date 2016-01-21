Introduction
============

OG SimpleStats provides some basic group statistics including total post count,
total member count, and total comment count (coming soon).  This module is meant
as a lightweight alternative to http://www.drupal.org/project/og_statistics.
og_statistics also has not released a Drupal 7 port yet, so this module serves
those sites looking for basic group statistics.

This module is also attempts to provide these statistics on-demand without
storing the data in a new schema. Organic Groups provides most of this data in
the og_membership table so a simple / single query can gather this.


Main Features
-------------

* Provides a Total Posts (extra field) on all group bundles. This can be
  rendered directly on the node/entity.
* Provides a Total Members (extra field) on all group bundles. This can be
  rendered directly on the node/entity.
* Provides a Views field handler for both Total Posts and Total Members.


Configuration
=============

There currently is no configuration.


Recommendations
===============

Apply the patch from http://www.drupal.org/node/1256368.
When enabling modules that use hook_field_extra_fields, those extra fields
automatically get added as visible to node displays. This patch allows them to
initially be disabled.


Maintainers
===========

* Craig Aschbrenner <https://www.drupal.org/user/246322>
