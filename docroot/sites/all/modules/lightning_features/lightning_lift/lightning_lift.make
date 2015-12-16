api = 2
core = 7.x

projects[acquia_connector][version] = "2.15"
projects[acquia_connector][type] = "module"
projects[acquia_connector][subdir] = "contrib"

projects[acquia_lift][version] = "2.0-beta9"
projects[acquia_lift][type] = "module"
projects[acquia_lift][subdir] = "contrib"

; Modal backdrop scrolls with page.
; https://www.drupal.org/node/2627192#comment-10627356
projects[acquia_lift][patch][2627192] = "http://drupal.org/files/issues/modal-background-scrolling-2627192-2.patch"

projects[personalize][version] = "2.x-dev"
projects[personalize][type] = "module"
projects[personalize][subdir] = "contrib"
projects[personalize][download][type] = "git"
projects[personalize][download][revision] = "4ad98b2"
projects[personalize][download][branch] = "7.x-2.x"

projects[visitor_actions][version] = "1.4"
projects[visitor_actions][type] = "module"
projects[visitor_actions][subdir] = "contrib"

; Libraries
libraries[chosen][destination] = "libraries"
libraries[chosen][download][type] = "get"
libraries[chosen][download][url] = "https://github.com/harvesthq/chosen/releases/download/v1.1.0/chosen_v1.1.0.zip"

;libraries[d3][destination] = "libraries"
;libraries[d3][download][type] = "get"
;libraries[d3][download][url] = https://github.com/mbostock/d3/releases/download/v3.4.11/d3.zip
; Note: D3 not whitelisted, must be downloaded manually.
; https://www.drupal.org/node/2481947

libraries[qtip][destination] = "libraries"
libraries[qtip][download][type] = "get"
libraries[qtip][download][url] = "https://raw.githubusercontent.com/Craga89/qTip1/master/1.0.0-rc3/jquery.qtip-1.0.0-rc3.min.js"

libraries[rickshaw][destination] = "libraries"
libraries[rickshaw][download][type] = "get"
libraries[rickshaw][download][url] = https://github.com/shutterstock/rickshaw/archive/v1.5.0.zip
