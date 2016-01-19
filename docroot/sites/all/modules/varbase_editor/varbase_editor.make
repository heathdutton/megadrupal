; Varbase Editor Makefile

api = 2
core = 7.x

; WYSIWYG Improvements

projects[ace_editor][version] = 1.3
projects[ace_editor][subdir] = contrib

projects[better_formats][version] = "1.x-dev"
projects[better_formats][download][type] = "git"
projects[better_formats][download][url] = "http://git.drupal.org/project/better_formats.git"
projects[better_formats][download][revision] = "3b4a8c92b317add4e218216a002b2bc777fbc736"
projects[better_formats][subdir] = contrib

projects[ckeditor][version] = "1.x-dev"
projects[ckeditor][download][type] = "git"
projects[ckeditor][download][url] = "http://git.drupal.org/project/ckeditor.git"
projects[ckeditor][download][revision] = "292de9321db65a606b9e2016251979379a3c6528"
projects[ckeditor][subdir] = contrib

projects[media_ckeditor][version] = "2.x-dev"
projects[media_ckeditor][download][type] = "git"
projects[media_ckeditor][download][url] = "http://git.drupal.org/project/media_ckeditor.git"
projects[media_ckeditor][download][revision] = "7409f2c0923f7bd81e91303a9d6032505d89d1cf"
projects[media_ckeditor][subdir] = contrib
projects[media_ckeditor][patch][2333855] = "http://www.drupal.org/files/issues/media_ckeditor-module-browser-tabs-2333855-13.patch"
projects[media_ckeditor][patch][2352573] = "http://www.drupal.org/files/issues/ckeditor_media_wysiwyg-2352573-7.patch"

projects[pathologic][version] = 2.12
projects[pathologic][subdir] = contrib

projects[token_filter][version] = 1.1
projects[token_filter][subdir] = contrib

projects[wysiwyg_filter][version] = 1.6-rc2
projects[wysiwyg_filter][subdir] = contrib
projects[wysiwyg_filter][patch][1783966] = "http://www.drupal.org/files/wysiwyg_filter_alter_blacklist-1783966-4.patch"

;;;;;;;;;;;;;;;;;;;;;
;; Libraries
;;;;;;;;;;;;;;;;;;;;;

libraries[ace][download][type] = "get"
libraries[ace][download][url] = "http://github.com/ajaxorg/ace-builds/archive/v1.2.0.tar.gz"

libraries[ckeditor][download][type] = "get"
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.5.5/ckeditor_4.5.5_full.zip"

libraries[widget][download][type] = "get"
libraries[widget][download][url] = "http://download.ckeditor.com/widget/releases/widget_4.5.5.zip"
libraries[widget][destination] = "modules/contrib/ckeditor/plugins/"

libraries[lineutils][download][type] = "get"
libraries[lineutils][download][url] = "http://download.ckeditor.com/lineutils/releases/lineutils_4.5.5.zip"
libraries[lineutils][destination] = "modules/contrib/ckeditor/plugins/"
