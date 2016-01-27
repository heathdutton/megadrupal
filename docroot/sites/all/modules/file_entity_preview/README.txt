Overview
========

Provides a widget for file fields which includes a preview of the rendered file
as configured in the File Entity module.

This allows edit forms to display thumbnails etc. like the core image field
widget. Used in combination with the File Entity Inline module, this avoids the
need to use the Media module's file widget when only dealing with uploaded
files, providing a much simpler more native feeling interface.

Usage
=====

- Configure the 'preview' display settings for each file type in the File
  Types page, under 'manage file display'.
- Choose the 'File entity preview' widget type for each file field where you
  would like previews displayed.
