core = 7.x
api = 2

; this makefile fetches the latest Aegir code from git from drupal.org
; it shouldn't really change at all apart from major upgrades, where
; the branch will change
projects[drupal][type] = "core"

; chain into hostmaster from git's 3.x branch
projects[hostmaster][type] = "profile"
projects[hostmaster][download][type] = "git"
projects[hostmaster][download][url] = "http://git.drupal.org/project/hostmaster.git"
projects[hostmaster][download][branch] = "7.x-3.x"

projects[hosting_valkyrie][type] = "module"
projects[hosting_valkyrie][download][type] = "git"
projects[hosting_valkyrie][download][url] = "http://github.com/GetValkyrie/hosting_valkyrie.git"
projects[hosting_valkyrie][download][branch] = "master"

projects[hosting_dev][version] = "1.0"

projects[hosting_s3][type] = "module"
projects[hosting_s3][download][type] = "git"
projects[hosting_s3][download][url] = "http://github.com/GetValkyrie/hosting_s3.git"
projects[hosting_s3][download][branch] = "master"
