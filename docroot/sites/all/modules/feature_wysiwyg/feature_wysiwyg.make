api = 2
core = 7.x

; Modules =====================================================================

projects[features][subdir] = contrib
projects[features][version] = 1.0-rc1

projects[imce][subdir] = contrib
projects[imce][version] = 1.5

projects[imce_wysiwyg][subdir] = contrib
projects[imce_wysiwyg][version] = 1.0

projects[strongarm][subdir] = contrib
projects[strongarm][version] = 2.0-beta5

projects[wysiwyg][subdir] = contrib
projects[wysiwyg][version] = 2.x-dev


; Libraries ==================================================================

libraries[ckeditor][download][type]= "get"
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%203.6.2/ckeditor_3.6.2.zip"
libraries[ckeditor][directory_name] = "ckeditor"
libraries[ckeditor][destination] = "libraries"