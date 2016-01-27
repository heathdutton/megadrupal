core = 6.x
api = 2

projects[drupal][type] = "core"
projects[drupal][patch][] = "http://drupal.org/files/robots_txt_rollback.patch"

; Download the OpenAtrium Install profile and recursively build all its dependencies:
projects[openatrium][type] = "profile"
projects[openatrium][download][type] = "git"
projects[openatrium][download][url] = "http://git.drupal.org/project/openatrium.git"
projects[openatrium][download][branch] = "6.x-1.x"

projects[atrium_scrum][download][type] = git
projects[atrium_scrum][download][url] = ergonlogic@git.drupal.org:project/atrium_scrum.git
projects[atrium_scrum][download][branch] = 6.x-1.x

projects[atrium_moderated_groups][download][type] = git
projects[atrium_moderated_groups][download][url] = ergonlogic@git.drupal.org:project/atrium_moderated_groups.git
projects[atrium_moderated_groups][download][branch] = master

projects[atrium_captchas][download][type] = git
projects[atrium_captchas][download][url] = ergonlogic@git.drupal.org:project/atrium_captchas.git
projects[atrium_captchas][download][branch] = master

projects[user_limit][download][type] = git
projects[user_limit][download][url] = ergonlogic@git.drupal.org:project/user_limit.git
projects[user_limit][download][branch] = 6.x-1.x
