; Radix Core Makefile

api = 2
core = 7.x

; Radix Theme

projects[radix][type] = theme
projects[radix][version] = 3.0-rc4

; Radix Modules

projects[radix_layouts][type] = module
projects[radix_layouts][download][type] = git
projects[radix_layouts][download][branch] = 7.x-3.x
projects[radix_layouts][subdir] = contrib

projects[radix_views][type] = module
projects[radix_views][download][type] = git
projects[radix_views][download][branch] = 7.x-1.x
projects[radix_views][subdir] = contrib
