core = 7.x
api = 2
; Modules
projects[better_formats][subdir] = "contrib"
projects[ctools][subdir] = "contrib"
projects[features][subdir] = "contrib"
projects[image_resize_filter][subdir] = "contrib"
projects[imce][subdir] = "contrib"
projects[imce_wysiwyg][subdir] = "contrib"
projects[strongarm][subdir] = "contrib"
projects[wysiwyg][subdir] = "contrib"
projects[wysiwyg][version] = "2.x-dev"



projects[wysiwyg_template][subdir] = "contrib"

libraries[ckeditor][download][type] = "git"
libraries[ckeditor][download][url] = "https://github.com/ckeditor/ckeditor-releases.git"
libraries[ckeditor][download][tag] = "full/4.4.0"
libraries[ckeditor][directory_name] = "ckeditor"
libraries[ckeditor][destination] = "libraries"

libraries[tinymce][download][type] = "git"
libraries[tinymce][download][url] = "https://github.com/tinymce/tinymce-dist.git"
libraries[tinymce][directory_name] = "tinymce"
libraries[tinymce][destination] = "libraries"
