INTRODUCTION
------------

Workbench Access IMCE provides a function, workbench_access_imce_path, that can
be used with the directory specifications in IMCE profiles along with the
Workbench Access section assignment to give people access to the same imce
folders as Workbench Sections. It is useful when users need to have access to
the same folders as the Workbench Sections. This way you don't need to create a
role for each user in imce.


REQUIREMENTS
------------

This module requires the following modules:
 * Workbench Access (https://www.drupal.org/project/workbench_access)
 * IMCE (https://www.drupal.org/project/imce)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Once the module is installed, go to Administration >> Configuration >>
   Workbench >> Workbench Access (Workbench Access IMCE Prefix section) to set
   the root folder that will have house all of the other folders.


TROUBLESHOOTING
---------------

The module will only work with the Taxonomy Active access scheme in Workbench
Access's settings. If someone wants to write the Menu Active access scheme,
that would be great.


MAINTAINERS
-----------

Nate Millin at WI DPI
