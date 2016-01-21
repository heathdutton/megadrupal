; Commerce Deploy Promotions makefile

api = 2
core = 7.x

; Contribs
projects[commerce_coupon][download][type] = git
projects[commerce_coupon][download][branch] = 7.x-2.x
projects[commerce_coupon][download][revision] = 1cf041a
projects[commerce_coupon][subdir] = contrib

projects[commerce_discount][download][type] = git
projects[commerce_discount][download][branch] = 7.x-1.x
projects[commerce_discount][download][revision] = a40f74a
; Patch from #2276335 require reroll.
; projects[commerce_discount][patch][2237193] = "https://www.drupal.org/files/issues/commerce_discount-2276335-1.patch"
projects[commerce_discount][subdir] = contrib

projects[commerce_discount_extra][download][type] = git
projects[commerce_discount_extra][download][branch] = 7.x-1.x
projects[commerce_discount_extra][download][revision] = ffaa613
projects[commerce_discount_extra][subdir] = "contrib"

projects[commerce_discount_product_category][version] = 1.x
projects[commerce_discount_product_category][download][type] = git
projects[commerce_discount_product_category][download][branch] = 7.x-1.x
projects[commerce_discount_product_category][download][revision] = d6073f4
projects[commerce_discount_product_category][subdir] = contrib
projects[commerce_discount_product_category][patch][2285275] = https://www.drupal.org/files/issues/add_product_category-2285275-4.patch

projects[entityreference][download][type] = git
projects[entityreference][download][branch] = 7.x-1.x
projects[entityreference][download][revision] = dc4196b
projects[entityreference][subdir] = contrib
projects[entityreference][patch][1823406] = https://www.drupal.org/files/undefined_index-1823406-14.patch

projects[inline_conditions][download][type] = git
projects[inline_conditions][download][branch] = 7.x-1.x
projects[inline_conditions][download][revision] = 3dd9495
projects[inline_conditions][subdir] = contrib
