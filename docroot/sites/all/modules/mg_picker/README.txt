== SUMMARY ==

This module creates a filter and a wysiwyg plugin to select galleries in content.


== DEPENDENCIES ==

- Media Gallery
- Wysiwyg (optional)


== INSTALLATION ==

1) Downdload Media Gallery module, and its dependencies
2) Download Wysiwyg module
3) Download one of the supported editors (for example TinyMCE)
4) Enable Media Gallery picker module
5) Enable the filter on input formats
6) Add button on wysiyg profile


== USAGE ==

After installation steps you can click to the wysiwg button, it craetes a dialog window. In the dialog you can select
one of the created galleries via an autocomplete input field.
After you selected the gallery, the dialog shows the gallery's images.
Select on of the images and press insert button.

The pattern:
[mg_picker:NID fid:FID]

fid attribute is optional. If fid attribute is missing, the generated link will shows the translatable "Go to gallery"
text, and it links to the gallery node page.
If fid attribute is added, the image will generated as a link to the gallery image.