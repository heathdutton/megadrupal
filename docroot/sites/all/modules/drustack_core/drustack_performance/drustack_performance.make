api = 2
core = 7.x

; Modules
projects[cdn][download][branch] = 7.x-2.x
projects[cdn][download][type] = git
projects[cdn][patch][2407287] = https://drupal.org/files/issues/2407287-Farfuture-incopatible-with-image-style-tokens-2.patch
projects[cdn][subdir] = contrib
projects[memcache][download][tag] = 7.x-1.5
projects[memcache][download][type] = git
projects[memcache][subdir] = contrib
projects[varnish][download][branch] = 7.x-1.x
projects[varnish][download][type] = git
projects[varnish][patch][2455293] = https://drupal.org/files/issues/varnish-syntax_error-2455293-1.patch
projects[varnish][subdir] = contrib
