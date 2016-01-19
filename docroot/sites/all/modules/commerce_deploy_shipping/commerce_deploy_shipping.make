; Commerce Deploy Shipping Makefile

api = 2
core = 7.x

; Commerce Shipping Contribs

projects[commerce_flat_rate][version] = 1.0-beta2
projects[commerce_flat_rate][subdir] = contrib

projects[commerce_shipping][version] = 2.2
projects[commerce_shipping][subdir] = contrib

projects[commerce_physical][download][type] = git
projects[commerce_physical][download][branch] = 7.x-1.x
projects[commerce_physical][download][revision] = e2a8866
projects[commerce_physical][subdir] = contrib

; Contrib dependencies

projects[physical][download][type] = git
projects[physical][download][branch] = 7.x-1.x
projects[physical][download][revision] = 32e1a38
projects[physical][subdir] = contrib
