Architecture v2.x
=================

This version of the Architecture module defines Views, using data exposed via
the Views API, that give any user with the "Use Architecture" permission the
ability to survey the structure of the site.

Security
========
Many of these queries bypass access restrictions, so assign the "Use
Architecture" permission with caution!

Data export
===========
Since it is built on Views, this module supports exporting the Views data via
the Views Data Export module, and can be used with all drush commands that come
with Views and any Views-extending modules that may be enabled.

Finding PHP in fields
=====================
There is also support for searching for PHP opening tags that may be saved in
fields. This is a common need but it also requires running a great number of
queries because each field's data is a separate table in Drupal. Therefore, no
page or block display is available for this view. Instead, it is encouraged to
use the drush command

$ drush phpaudit

or the abbreviated version,

$ drush apa

to get this information, as attempts to access it via the
UI may fall over, particularly on sites with a great number of fields.

License
=======
Copyright (C) 2013  Sheldon Rampton and Beth Binkovitz

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
