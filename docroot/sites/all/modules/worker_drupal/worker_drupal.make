; Drush Make (http://drupal.org/project/drush_make)
api = 2

; Drupal core

core = 7.x
projects[drupal] = 7

; Dependencies

projects[browser] = 1
projects[coder] = 1

projects[worker][type] = module
projects[worker][download][type] = git
projects[worker][download][url] = http://git.drupal.org/project/worker.git
projects[worker][download][branch] = 7.x-1.x
