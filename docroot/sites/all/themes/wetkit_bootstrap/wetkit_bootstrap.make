; WetKit Bootstrap Makefile

core = 7.x
api = 2

; Theme(s)

projects[bootstrap][version] = 3.1-beta2
projects[bootstrap][type] = theme
projects[bootstrap][patch][2098551] = http://drupal.org/files/issues/registry_alter_does_not-2098551-42.patch
projects[bootstrap][patch][2311463] = http://drupal.org/files/issues/patch_bootstrap_wetkit-2311463-05.patch
projects[bootstrap][patch][2369235] = http://drupal.org/files/issues/bootstrap_password_policy-2369235-01.patch
projects[bootstrap][patch][2404405] = http://drupal.org/files/issues/bootstrap-modal-nav-offset-jslint-2404405-2.patch
projects[bootstrap][patch][2416485] = http://drupal.org/files/issues/preg_grep_in-2416485-4.patch
