
ABOUT THE CIVICRM TAXONOMY GROUP MODULE
---------------------------------------

The module enables for taxonomy vocabularies and terms within Drupal to be
syncronised with groups in CiviCRM.

The initial development of the Drupal 7.x version was sponsored by Nomensa
(http://www.nomensa.com).

INSTALLATION
------------

See http://drupal.org/documentation/install/modules-themes.

PERMISSIONS
-----------

The module creates a 'administer civicrm taxonomy groups' permission that is
required to view the admin settings page.

USAGE
-----

 * Download and enable the module.
 * Create your taxonomy vocabularies and terms.
 * Go to the settings page at /admin/config/civicrm-taxonomy and enable the
   required vocabularies.
 * Once the form is submitted, a new group in CiviCRM is created, as well as
   for any terms within the vocabulary.
 * Any additions/updates/deletions to the vocabulary or its terms whilst it is
   enabled on this page will automatically be reflected within the
   corresponding group in CiviCRM.
 * If the vocabulary is later disabled, then all the corresponding groups are
   removed from CiviCRM.

AUTHOR
------

Oliver Davies
oliver@oliverdavies.co.uk
