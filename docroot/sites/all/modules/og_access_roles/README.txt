INTRODUCTION
------------

The OG Access Roles module extends the reach of Organic Groups' bundled 'Organic
groups access control' module by allowing node authors finer control over who
can see their posts.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/og_access_roles

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/og_access_roles


REQUIREMENTS
------------
This module requires the following modules:

 * OG (https://drupal.org/project/og)
 * OG Access (sub-module of the OG module)


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

 * This module makes 2 new fields available on the OG Field Settings
   administration page that is provided by the OG module, which can be found at
   admin/config/group/fields.
   The fields are:

   - Group visibility roles

     A field to select which user roles can access a group node.
     Users with that roll will be granted access even if they are not a member
     of that group.

   - Group content visibility roles

     A field to select which user roles can access a group content node.
     Users with that roll will be granted access even if they are not a member
     of the group the content is in.

   Add these fields to your content types as you would do with other OG fields.
   The fields will only affect access on groups that are set to private and
   group content that is set to private or is set to inherit the group's
   permissions and in part of a private group.
