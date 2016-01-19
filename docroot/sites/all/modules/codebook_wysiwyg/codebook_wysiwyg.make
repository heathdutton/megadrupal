; Municipal Code - WYSIWYG Makefile

api = 2
core = 7.x

projects[ckeditor][subdir] = contrib
projects[entity_view_mode][subdir] = contrib

libraries[ckeditor][download][type] = get
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.4.3/ckeditor_4.4.3_full.zip"
libraries[ckeditor][destination] = libraries
libraries[ckeditor][directory_name] = ckeditor
