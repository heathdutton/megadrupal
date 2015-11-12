; Open Atrium Folders multiple upload feature from 2.23
; drush make --no-core sites/all/modules/oa_folders/oa_folders_plupload/oa_folders_plupload.make .

api = 2
core = 7.x

projects[oa_plupload][type] = module
projects[oa_plupload][subdir] = contrib
projects[oa_plupload][download][url] = https://github.com/phase2/oa_plupload.git
projects[oa_plupload][download][type] = git
projects[oa_plupload][download][branch] = 7.x-2.x
projects[oa_plupload][patch][] = oa_plupload.patch
