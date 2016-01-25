; WetKit Bootstrap Makefile

core = 7.x
api = 2

; Theme(s)

projects[bootstrap][version] = 3.4
projects[bootstrap][type] = theme
projects[bootstrap][patch][2102895] = http://drupal.org/files/issues/bootstrap-sass-starterkit-2102895-78.patch
projects[bootstrap][patch][2311463] = http://drupal.org/files/issues/patch_bootstrap_wetkit-2311463-05.patch
projects[bootstrap][patch][2556733] = http://drupal.org/files/issues/update_to_bootstrap-2556733-14.patch
projects[bootstrap][patch][2579657] = http://drupal.org/files/issues/ajax_throbber_image_not-2579657-2.patch
