; WetKit WYSIWYG Makefile

api = 2
core = 7.x

; Contrib

projects[linkit][version] = 3.5
projects[linkit][subdir] = contrib
projects[linkit][patch][2381549] = http://drupal.org/files/issues/linkit_title_and_uuid-2381549-10.patch

projects[wysiwyg][version] = 2.x-dev
projects[wysiwyg][subdir] = contrib
projects[wysiwyg][download][type] = git
projects[wysiwyg][download][revision] = 37dc07d
projects[wysiwyg][download][branch] = 7.x-2.x
projects[wysiwyg][patch][1786732] = http://drupal.org/files/wysiwyg-arbitrary_image_paths_markitup-1786732-3.patch

; Include our Editors

libraries[ckeditor][download][type] = get
libraries[ckeditor][download][url] = http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.3.5/ckeditor_4.3.5_standard.zip

libraries[markitup][download][type] = get
libraries[markitup][download][url] = https://github.com/markitup/1.x/tarball/master
libraries[markitup][patch][1715642] = http://drupal.org/files/1715642-adding-html-set-markitup-editor.patch
