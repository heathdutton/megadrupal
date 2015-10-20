api = 2
core = 6.x

;; Drupal & Hostmaster

projects[drupal][type] = "core"

projects[hostmaster][type] = "profile"
projects[hostmaster][download][type] = "git"
projects[hostmaster][download][url] = "http://git.drupal.org/project/hostmaster.git"
projects[hostmaster][download][branch] = "6.x-2.x"


;Devel tools
projects[] = devel

;; Aegir contrib
;;
;; To have new Aegir contrib modules added here, please file an issue under the
;; 'Makefiles' component here: http://drupal.org/project/issues/aegir-up, using

;; Production-ready
projects[] = "hosting_backup_queue"
projects[] = "hosting_backup_gc"

;; Testing

projects[] = "hosting_notifications"
projects[] = "hosting_server_titles"

;; Queued for testing
;;projects[] = "hosting_remote_import"
;;projects[] = "remote_import"
;hosting Drupal Gardens import: http://dirupal.org/sandbox/darthsteven/1178192
;http://drupal.org/project/aegir_feeds
;;projects[] = "hosting_platform_pathauto"
;hosting drush make sources: http://drupal.org/sandbox/darthsteven/1141164
;hosting site admin name: http://drupal.org/sandbox/darthsteven/1201092
;http://drupal.org/project/hosting_services
;http://drupal.org/project/aegir_rules
;http://drupal.org/project/hosting_site_git
;provision backup platform: http://drupal.org/sandbox/helmo/1283656
;drush remake: http://drupal.org/sandbox/darthsteven/1432518

;; ecommerce integration

;projects[] = "ubercart"
;projects[] = "uc_hosting"
;projects[] = "hosting_profile_roles"
;projects[] = "token"


