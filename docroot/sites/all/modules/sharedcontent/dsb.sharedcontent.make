; ----------------
; Drush Make file to set up a Shared Content capable drupal environment.
; ----------------

api = 2
core = 7.x
projects[drupal][version] = 7.22

; Dependency modules for Shared Content.
; --------

projects[sharedcontent][type] = module
projects[sharedcontent][download][type] = git
projects[sharedcontent][download][url] = http://git.drupal.org/project/sharedcontent.git
projects[sharedcontent][download][branch] = 7.x-1.x

; Dependency modules for the testing environment
; --------

projects[devel] = 1.3

projects[past] = 1.0-alpha1

; Use search_api_db to avoid external dependencies. As this backend does not
; support feature 'search_api_mlt' (http://drupal.org/node/1254698, 2013-05-01),
; the suggestions tab is not displayed.
projects[search_api_db] = 1.x-dev
