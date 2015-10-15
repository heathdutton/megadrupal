; Commerce Deploy Product Makefile

api = 2
core = 7.x

; Contribs

; We have to mark a version, otherwise dependencies will be angry (taken from tar.)
projects[inline_entity_form][version] = 1.6
projects[inline_entity_form][subdir] = contrib
projects[inline_entity_form][patch][1777254] = http://drupal.org/files/issues/inline_entity_form-implementation-1777254-13.patch

; Commerce Contribs

projects[commerce_autosku][download][type] = git
projects[commerce_autosku][download][revision] = 562b9a5
projects[commerce_autosku][download][branch] = 7.x-1.x
projects[commerce_autosku][subdir] = contrib
projects[commerce_autosku][patch][2229007] = https://www.drupal.org/files/issues/fix_empty_check-2229007.patch

projects[commerce_custom_product][version] = 1.0-beta2
projects[commerce_custom_product][subdir] = contrib

projects[commerce_extra_price_formatters][version] = 1.1
projects[commerce_extra_price_formatters][subdir] = contrib
