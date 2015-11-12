; WetKit Widgets Makefile

api = 2
core = 7.x

projects[file_entity][version] = 2.0-beta2
projects[file_entity][subdir] = contrib
projects[file_entity][patch][2000934] = http://drupal.org/files/issues/allow_selection_of-2000934-30.patch
projects[file_entity][patch][2198973] = http://drupal.org/files/issues/file_entity_override_widgets-2198973-01.patch

projects[file_lock][version] = 2.0
projects[file_lock][subdir] = contrib

projects[media][version] = 2.0-beta1
projects[media][subdir] = contrib
projects[media][patch][2084287] = http://drupal.org/files/issues/media-file-name-focus-2084287-2.patch
projects[media][patch][2187771] = http://drupal.org/files/issues/media_alt_attributes-2129273-24.patch
projects[media][patch][2126697] = http://drupal.org/files/issues/media_wysiwyg_2126697-53.patch
projects[media][patch][2272567] = http://drupal.org/files/issues/media_dialog_appears-2272567-8.patch
projects[media][patch][2308487] = http://drupal.org/files/issues/media-alt-title-double-encoded-2308487-2.patch
