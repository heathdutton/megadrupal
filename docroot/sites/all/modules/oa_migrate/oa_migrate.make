; Open Atrium Migrate Makefile

api = 2
core = 7.x

; Migrate
projects[migrate][version] = 2.x-dev
projects[migrate][subdir] = contrib
projects[migrate][download][type] = git
projects[migrate][download][branch] = 7.x-2.x
projects[migrate][download][revision] = 0010411

; Migrate Drupal-to-Drupal
projects[migrate_d2d][version] = 2.x-dev
projects[migrate_d2d][subdir] = contrib
projects[migrate_d2d][download][type] = git
projects[migrate_d2d][download][branch] = 7.x-2.x
projects[migrate_d2d][download][revision] = 55662ed
projects[migrate_d2d][patch][1834016] = https://www.drupal.org/files/issues/migrate_d2d-node-revisions-1834016-5.patch
