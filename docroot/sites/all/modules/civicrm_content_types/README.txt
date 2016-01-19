
ABOUT THIS MODULE
-----------------

The module enables for content types within Drupal to be syncronised with
groups in CiviCRM.

The initial development of this module was sponsored by Nomensa
(http://www.nomensa.com).

INSTALLATION
------------

See http://drupal.org/documentation/install/modules-themes.

PERMISSIONS
-----------

The module creates a 'administer civicrm content type groups' permission that
is required to view the admin settings page.

USAGE
-----

 * Download and enable the module.
 * Create your content types within Drupal.
 * Go to the settings page at /admin/config/civicrm-types and enable the
   required content tyeps.
 * Once the form is submitted, a new group in CiviCRM is created for each
   enabled content type.
 * If the content type is later disabled, then its corresponding group is
   removed from CiviCRM.

AUTHOR
------

Oliver Davies
oliver@oliverdavies.co.uk
