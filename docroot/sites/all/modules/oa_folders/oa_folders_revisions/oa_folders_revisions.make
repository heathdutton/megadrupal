; Open Atrium Folders file revision

api = 2
core = 7.x

; projects[ctools_automodal][subdir] = contrib
projects[ctools_automodal][version] = 1.x-dev

; projects[file_admin][subdir] = contrib
projects[file_admin][version] = 1.x-dev

; projects[file_entity_revisions][subdir] = contrib
projects[file_entity_revisions][version] = 1.x-dev
; Add revisions tabs
; http://drupal.org/node/2097975
projects[file_entity_revisions][patch][2097975] = https://www.drupal.org/files/issues/file_entity_revisions-revisions_tab-2097975-3.patch

; fix permission for private files
; https://www.drupal.org/node/2383231
projects[file_entity_revisions][patch][2383231] = https://www.drupal.org/files/issues/private_file_revisions-2383231-1.patch.patch
