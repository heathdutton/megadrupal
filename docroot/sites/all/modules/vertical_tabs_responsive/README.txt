CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Recommended modules
 * Installation
 * Configuration

INTRODUCTION
------------

This module responsifies vertical tabs in admin pages. You will have Drupal 8
like vertical tabs with only enabling it:

 * For desktop resolutions vertical tabs will appear in a right sidebar.
   However, you can also choose vertical tabs to appear on left sidebar
   through module'sadministration page.

 * For mobile resolutions vertical tabs will appear at the bottom of the page.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/vertical_tabs_responsive

RECOMMENDED MODULES
-------------------

 * Field Group (https://www.drupal.org/project/field_group):
   Allows you to group fields together by using "Fieldsets", "Horizontal tab",
   "Vertical tabs", etc. Notice if you create your custom vertical tabs for a
   content type with Field group, Vertical Tabs Responsive will render all your
   custom vertical tabs together with drupal's standart vertical tabs!

 * Node form columns (https://www.drupal.org/project/nodeformcols):
   This module doesn't responsify vertical tabs but it allows you to put fields
   (including vertical tabs) on the right side or in the footer of node form
   pages. This module can't be used together with Vertical Tabs Responsive.

 * Responsive Vertical Tabs (https://www.drupal.org/sandbox/tkoleary/2236189):
   This module is still a sandbox but it does almost the same as Vertical
   Tabs Responsive. Some of this module's code is based on this project!
   Thanks tkoleary!

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

 * Configure module's options in Administration » Configuration » User Interface
   » Vertical Tabs Responsive:

    - Append show/hide button to vertical tabs top.

      Allows you to enable a button which will be appended to vertical tabs.
      This button will allow you to dynamically hide and show vertical tabs.

    - Collapse all vertical tabs by default.

      If this option is enabled, all vertical tabs get collapsed on page load.

    - Vertical tabs side

      Allows to choose in which side the vertical tabs will be on desktop
      screens.

    - Vertical tabs width

      Allows to choose width of content and vertical tabs section (in
      percentage).

    - Reponsive breakpoint

      Allows to choose the width which sets the breakpoint between mobile and
      desktop versions of vertical tabs.

    - Blacklist: disable this module on the following paths

      Allows you to specify a list of paths where this module should be
      disabled.

    - Select roles that will see responsified vertical tabs

      Allows you to specify roles that will enjoy responsive vertical tabs.
