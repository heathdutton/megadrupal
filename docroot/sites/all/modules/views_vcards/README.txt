-- SUMMARY --

The Views vCards module allows the exporting of user fields in a vCard format.
This is mainly done because there was no way to export core profile fields to a
vCard format without user intervention.


-- REQUIREMENTS --

* Views; https://www.drupal.org/project/views
* Libraries API; https://www.drupal.org/project/libraries


-- INSTALLATION --

* Copy the whole views_vcards directory to your modules directory
  (DRUPAL_ROOT/sites/all/modules) and activate the Views vCards module on
  http://www.example.com/admin/modules.

* Download the ZipStream-PHP library from
  https://github.com/maennchen/ZipStream-PHP and place it in
  (DRUPAL_ROOT/sites/all/libraries) so that the file zipstream.php is
  reachable at (DRUPAL_ROOT/sites/all/libraries/zipstream/zipstream.php)


-- CONFIGURATION --

* Create or edit a View showing users, then create a new display of type
  'vCard'. In this feed choose vCards for 'Format' and 'Show'. The settings
  dialog for 'Show' will allow you to select what user field should be used
  for each vCard property.

    - Make sure the fields you want to use are added in the fields section,
      otherwise they will not show up.

* Views vCards offers the 'attach to' option under vCard settings, allowing
  this module to add a download link to another page. You can set up your user
	list as normal with all (exposed) filters if you require them, and the vCard
  export will adapt to this and provide the right vCards.
