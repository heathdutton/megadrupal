; Commerce Deploy Checkout Makefile

api = 2
core = 7.x

; Contribs

projects[admin_views][version] = 1.5
projects[admin_views][subdir] = contrib

projects[charts][version] = 2.0-rc1
projects[charts][subdir] = contrib

projects[commerce_backoffice][version] = 1.4
projects[commerce_backoffice][subdir] = contrib

projects[commerce_reports][version] = 4.0-beta2
projects[commerce_reports][subdir] = contrib

projects[eva][download][type] = git
projects[eva][download][branch] = 7.x-1.x
projects[eva][download][revision] = 5798231
projects[eva][subdir] = contrib

projects[module_filter][version] = 2.0
projects[module_filter][subdir] = contrib

projects[navbar][version] = 1.6
projects[navbar][subdir] = contrib

projects[views_data_export][version] = 3.0-beta8
projects[views_data_export][subdir] = contrib

projects[views_date_format_sql][version] = 3.1
projects[views_date_format_sql][subdir] = contrib

projects[views_megarow][version] = 1.4
projects[views_megarow][subdir] = contrib

projects[commerce_admin_order_advanced][download][type] = git
projects[commerce_admin_order_advanced][download][branch] = 7.x-1.x
projects[commerce_admin_order_advanced][download][revision] = 609b767
projects[commerce_admin_order_advanced][subdir] = contrib

; Admin theme

projects[shiny][version] = 1.7

; Libraries

libraries[backbone][download][type] = get
libraries[backbone][download][url] = https://github.com/jashkenas/backbone/archive/1.0.0.tar.gz

libraries[underscore][download][type] = get
libraries[underscore][download][url] = https://github.com/jashkenas/underscore/archive/1.5.2.zip
