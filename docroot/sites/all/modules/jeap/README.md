INTRODUCTION
------------
This module allows the site builder to give day-to-day site administrators (who
may be one and the same people) a better administration experience by removing
irrelevant configuration options.

It does this simply with the power of Drupal's

It builds on the Custom Permissions (config_perms) module and so inherits its
main feature: your ability as site builder to further grant just the access
you wish, based on paths.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/jeap


REQUIREMENTS
------------
This module requires the following module:
 * Custom Permissions (https://www.drupal.org/project/config_perms)


INSTALLATION
------------

Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7 for
further information.


CONFIGURATION
-------------

0. Ensure the role for which you want to give an simpler administrating
experience does *not* have the permission "Administer site configuration".
That gives people in that role access to all configuration, and a main point of
Just Enough Administrator Power is exclude administrators, distinguished from
site builders, from access to complex and possibly dangerous configuration.

1. Add the permissions you *do* want your simple administrator role to have on
Drupal's usual permissions page, Administration » People » Permissions
(admin/people/permissions).  You'll find key ones for exposing chosen parts of
Drupal's configuration pages under "Custom Permissions".

For instance, to allow a role to edit the site name and slogan, give that role
the following three permissions:
 - 'display site configuration menu'
 - 'display site configuration system menu'
 - 'administer site information'

In addition to the path-based permissions automatically defined by Custom
Permissions and added to by this module (in the example above, the second
permission is from this module), you can define your own path permissions at
Administration » People » Custom Permissions (admin/people/custom_permissions).


MAINTAINERS
-----------

Principal author is Benjamin Melançon of Agaric.
  - https://drupal.org/u/mlncn
  - http://agaric.com

The author can be contacted for paid customizations of this module as well as
other Drupal consulting, development, and training.


FURTHER CREDIT
--------------

This module relies entirely on the Custom Permissions module created by mrthumpz
- https://www.drupal.org/user/266351 - and principally written by Sjoerd
Arendsen (Docc) - https://www.drupal.org/user/310132