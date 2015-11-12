
-- SUMMARY --
This module adds an extra option for Administration menu to exclude it for
certain paths. This might be useful sometimes for example if you are using
Civicrm which loads its own menu.

This module is sponsored by Leukaemia & Lymphoma Research

-- REQUIREMENTS --

Administration menu module

-- INSTALLATION --

* Install as usual, see
  https://www.drupal.org/documentation/install/modules-themes/modules-7
  for further information.

-- CONFIGURATION --

* Go to admin/config/administration/admin_menu. There you will have a new tab
  called Exclude. Add each url in a different line. You can use * at the end
  of the url as a wildcard.
