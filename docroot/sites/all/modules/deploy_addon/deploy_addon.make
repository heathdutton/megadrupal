; Drush make file for Deploy Add-on dependencies.

api = 2
core = "7.x"

projects[] = drupal

projects[deploy][subdir] = "contrib"
projects[deploy][version] = "2.0-beta1"

projects[entity_dependency][subdir] = "contrib"
projects[entity_dependency][version] = "1.0-alpha2"

projects[entity_menu_links][subdir] = "contrib"
projects[entity_menu_links][version] = "1.0-alpha3"

projects[services][subdir] = "contrib"
projects[services][version] = "3.12"

projects[webform_uuid][subdir] = "contrib"
projects[webform_uuid][version] = "1.1"

projects[uuid][subdir] = "contrib"
projects[uuid][version] = "1.0-beta1"
; Field collection - patch uuid services to not check access callback for field collections
; need a better way to do this but for now:
projects[uuid][patch][fieldcollection][url] = "http://drupal.org/files/uuid_services_field_collection_revisions.patch"
projects[uuid][patch][fieldcollection][md5] = "2dbf0fa15f0c30a5c17759b5810d65f8"

; Support for Redirect UUID.
projects[uuid_extras][subdir] = "contrib"
projects[uuid_extras][version] = "1.0-alpha2"
