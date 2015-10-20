api = 2
core = 6.x

;; Drupal & Hostmaster

projects[drupal][type] = "core"

projects[hostmaster][type] = "profile"
projects[hostmaster][download][type] = "git"
projects[hostmaster][download][url] = "http://git.drupal.org/project/hostmaster.git"
projects[hostmaster][download][branch] = "6.x-1.x"

;; Valkyrie
projects[valkyrie][type] = "module"
projects[valkyrie][download][type] = "git"
projects[valkyrie][download][url] = "http://git.drupal.org/project/valkyrie.git"
projects[valkyrie][download][branch] = "6.x-1.x"

;; DevShop & Aegir contrib
projects[devshop_dl][type] = "module"
projects[devshop_dl][download][type] = "git"
projects[devshop_dl][download][url] = "http://git.drupal.org/project/devshop_dl.git"
projects[devshop_dl][download][branch] = "6.x-1.x"
projects[hosting_logs][type] = "module"
projects[hosting_logs][download][type] = "git"
projects[hosting_logs][download][url] = "http://git.drupal.org/project/hosting_logs.git"
projects[hosting_logs][download][branch] = "6.x-1.x"
projects[hosting_drush_aliases][type] = "module"
projects[hosting_drush_aliases][download][type] = "git"
projects[hosting_drush_aliases][download][url] = "http://git.drupal.org/project/hosting_drush_aliases.git"
projects[hosting_drush_aliases][download][branch] = "6.x-1.x"
projects[] = "hosting_upload"
projects[] = "hosting_filemanager"
projects[] = "hosting_notifications"
projects[] = "hosting_server_titles"

;; Other contrib
projects[gitbrowser][type] = "module"
projects[gitbrowser][download][type] = "git"
projects[gitbrowser][download][url] = "http://git.drupal.org/project/gitbrowser.git"
projects[gitbrowser][download][branch] = "6.x-1.x"
projects[] = "token"
projects[] = "prod_check"

;; Wishlist
;projects[] = "helpful"
;projects[] = "hosting_solr"
;projects[] = "hosting_solr_devshop"
;varnish
;newrelic
;nagios
;nlg


; Development tools
projects[] = devel

