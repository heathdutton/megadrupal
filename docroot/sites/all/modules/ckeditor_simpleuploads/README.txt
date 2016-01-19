
Drupal 7 module to provide support for the SimpleUploads CKEditor plugin.
--------------------------------------
This module provides basic support for the SimpleUploads CKEditor plugin:
http://alfonsoml.blogspot.com/p/simpleuploads-plugin-for-ckeditor.html
The SimpleUploads plugin provides drag and drop uploading and 
inline placement of those images in CKEditor. 

This module also supports free version of simpleuploads, imagepaste that only 
works on Firefox web browser.

Basic installation steps are below.
For a more comprehensive set of steps visit:
http://adamkempler.com


### Installation and Setup 

You can choose between either ckeditor or wysiwyg (+ckeditor) module

** Using WYSIWYG (+CKEditor) module

- Make sure you have WYSIWYG module installed and setup with CKEditor plugin.
- Install either SimpleUploads or ImagePaste plugin to the ckeditor drupal module plugins folder.
- Enable the plugin in whichever wysiwyg profiles you want it available.
- Enable this module.
- Configure this module at /admin/config/content/simpleuploads
- Give the "Use SimpleUploads" permission to the relevant roles.


** Using CKEditor module

- Make sure you have CKEditor installed and setup.
- Install the SimpleUploads plugin to the ckeditor drupal module plugins folder.
- Edit the ckeditor drupal module's ckeditor.config.js and add the following lines:
-- config.extraPlugins = 'simpleuploads';
-- config.filebrowserUploadUrl = 'upload.php';
- Enable the plugin in whichever ckeditor profiles you want it available.
- Enable this module.
- Configure this module at /admin/config/content/simpleuploads
- Give the "Use SimpleUploads" permission to the relevant roles.


Maintainers
-----------
- Adam Kemper http://adamkempler.com
- Jānis Bebrītis http://wunderkraut.com