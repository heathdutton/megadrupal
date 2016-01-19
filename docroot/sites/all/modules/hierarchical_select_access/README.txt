INTRODUCTION
------------
The Hierarchical Select Access module prevents certain roles from accessing the
enhanced hierarchical menu, provided by the Hierarchical Select Menu module,
in content types.

You need this module if you have installed the Hierarchical Select module and
you want to prevent certain roles from selecting menu items in a hierarchy.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/drupalove/2448299


 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/2448299


REQUIREMENTS
------------
This module requires the following modules:
 * Hierarchical Select (https://www.drupal.org/project/hierarchical_select)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


   CONFIGURATION
   -------------
 * Customize the hierarchical select access settings in Administration »
   Configuration » Content authoring Hierarchical Select » Limit access.


- Tick the checkboxes next to the roles that should not see the hierarchical
     select menu widget.


- (Important) You must tick the checkboxes next to the content types that will
     be affected..


TROUBLESHOOTING
---------------
 * If you ever have problems, make sure to go through these steps:


   - Go to admin/reports/status (i.e. the Status Report). Ensure that the status
     of the Hierarchical Select by Role module is ok.


- Ensure that the page isn't being served from your browser's cache.
     Use CTRL+R in Windows/Linux browsers, CMD+R in Mac OS X browsers to enforce
     the browser to reload everything, preventing it from using its cache.


- Go to admin/config/development/performance and clear all caches.
