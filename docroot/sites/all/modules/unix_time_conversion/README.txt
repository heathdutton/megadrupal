*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

                            UNIX TIME CONVERSION

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

                            Author : Kunal Kursija

                Supporting organization: Blisstering Solutions

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

CONTENTS OF THIS FILE:
---------------------

- Introduction
- Requirements
- Installation
- permissions
- User Interface
- Un-installation
- Maintainers

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

INTRODUCTION:
------------

Unix Time Conversion - Provides very simple ajax blocks in user interface, from 
where users can convert timestamps to date & vice-versa.

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

REQUIREMENTS:
------------

As for now there are no requirements other than Drupal-Core.

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

INSTALLATION:
------------

To install this module
1) Go to admin/modules
2) Find 'Unix Time Conversion'
3) Check the box on left and save.

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

PERMISSIONS:
-----------

This module defines a permission named 'Configure unix time coversion'.
Users with this permission can control various administrative settings.
To alter the permissions of this page go to 'admin/people/permissions'.

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

USER INTERFACE:
--------------

1) Blocks for users:

After the module installation is complete, two blocks named 
- 'Date To Unix Timestamp'
- 'Unix Timestamp To Date'
will be automatically listed under 'admin/structure/block'.

These blocks can be enabled in any desired region of your theme.

2) Admin configuration page:

A page for admins is created at 'admin/config/regional/unix-time-conversion/
settings'.

Users with permission 'Configure unix time coversion' can access this page &
perform various administrative operations like changing titles, e.t.c.


*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

UN-INSTALLATION:
---------------

To un-install the module,
1) Go to admin/modules
2) Find Unix Time Conversion
3) Un-check the checkbox and save.
4) Then go to admin/modules/uninstall
5) Un-check the checkbox of Unix Time Conversion module and click 'uninstall'.

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

MAINTAINERS:
-----------

Current maintainers:
 * Kunal Kursija - https://www.drupal.org/user/2126548

This project has been sponsored by:
 * Blisstering Solutions: Drupal Services, Solutions and Products Company.

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

                                Thank You !
