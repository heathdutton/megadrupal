Views Per-User Cache
====================

This module provides a Views caching plugin that caches Views results and
displays on a per-user basis. This differs from the built-in time-based cache
plugin that will cache a View for the first user who loads the View, and then
displays it for all users. This cache plugin is useful for Views where the
contents are unique per-user due to Views filters or node access grants. This
cache plugin is best used for Views that are commonly displayed to
authenticated users such as on site homepages or sidebars.

Installation
============

Download this module to your modules directory (usually sites/all/modules), and
enable it at admin/modules. Once enabled, a new "Time-based, per user" cache
type will be available for in the "Other" section of each View.

