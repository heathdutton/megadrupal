SCALD OG
---------------------------------

Homepage:
http://drupal.org/sandbox/nagy.balint/1981744

Contents of this file:

 * Introduction
 * Installation

INTRODUCTION
------------

This module provides Organic groups integration for the scald module. It
creates OG permissions for each atom action. Does not currently support the
atom creation action.
This module can only extend permissions. Limiting permissions is not possible
with this module to let other modules fine tune the permission system.

INSTALLATION
------------

Scald OG depends on the following modules:

- Scald development version after 2013-May-24

- Organic groups 2.x branch

Installation steps:

1. Add an audience field to the atom entities at admin/config/group/fields

2. Edit the permissions at admin/config/group/permissions for the
specific group.

3. Optionally, on the dnd library view the new audience field can be
set up as a contextual filter to make sure that the library will only
show atoms that are part of the actual group.
