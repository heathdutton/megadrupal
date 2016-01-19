
#Drupal CKEditor Media Widget
This is 80% overwritten version of https://www.drupal.org/project/media_ckeditor

###Requirements:

https://www.drupal.org/project/media version 2.x

CKEditor 4.4 or newer

CKEditor Lineutils plugin

CKEditor Widget plugin

###File entity Display must be configured to show image at WYSIWYG view mode without any links.

admin/structure/file-types/manage/image/file-display/wysiwyg

admin/structure/file-types/manage/video/file-display/wysiwyg

admin/structure/file-types/manage/audio/file-display/wysiwyg

admin/structure/file-types/manage/document/file-display/wysiwyg

###Make sure that WYSIWYG view mode is in use inside a CKEditor.

admin/config/media/wysiwyg-view-mode

At CKEditor profile select: Plugin for embedding files using Media CKEditor Widget
