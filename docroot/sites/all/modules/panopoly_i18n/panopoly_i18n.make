; Drupal version
core = 7.x
api = 2

; Contrib modules
projects[variable][version] = 2.5
projects[variable][subdir] = contrib

projects[i18n][version] = 1.12
projects[i18n][subdir] = contrib
; Drupal 7.35 fix
projects[i18n][patch][2455093] = https://www.drupal.org/files/issues/2455093_drupal735-1.patch