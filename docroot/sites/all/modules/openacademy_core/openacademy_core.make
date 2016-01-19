; Open Academy Core Makefile

api = 2
core = 7.x

; ----------------------------------------------------------------------------
; OPEN ACADEMY CORE MODULES

projects[ds][version] = 1.9
projects[ds][type] = module
projects[ds][subdir] = contrib

projects[email][version] = 1.2
projects[email][type] = module
projects[email][subdir] = contrib

projects[phone][version] = 1.x-dev
projects[phone][type] = module
projects[phone][subdir] = contrib
projects[phone][download][type] = git
projects[phone][download][revision] = 173dd71
projects[phone][download][branch] = 7.x-1.x

projects[fontyourface][version] = 2.8
projects[fontyourface][subdir] = contrib
projects[fontyourface][patch][1913976] = http://drupal.org/files/fontyourface-Missing_font_preview_potentially_confusing-1913976-1.patch

projects[office_hours][version] = 1.3
projects[office_hours][type] = module
projects[office_hours][subdir] = contrib

projects[taxonomy_menu][version] = 1.2
projects[taxonomy_menu][subdir] = contrib
projects[taxonomy_menu][type] = module
projects[taxonomy_menu][patch][1486510] = http://drupal.org/files/taxonomy-menu-no-vid-checking.patch
