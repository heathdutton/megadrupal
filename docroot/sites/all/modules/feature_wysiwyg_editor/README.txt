**********************
** Permissions
Set the permission 'Use the WYSIWYG text format' at /admin/people/permissions for each role.

Turn all the better_formats permissions off for each role.

**********************
** Limit specific fields to the wysiwyg editor.

In the content type edit a field and set the following options.
e.g. /admin/structure/types/manage/NODETYPE/fields/body

Turn on 'Limit allowed text formats'.
Enable the 'wysiwyg' format and disable all others.

Turn on 'Overide the default order'.
Move WYSIWYG to the top of the list.

**********************
** Notes for the omega theme
Currently the wysiwyg editor picks up the styles from the base theme, to counter this you can set the 'Editor css' option to 'Define CSS' and point the 'CSS path' it straight to the global.css file of your custom theme. 

i.e. sites/all/themes/custom/css/global.css


Make file

BOOTSTRAP

************************
** Using the media module

Here is the recommended filter order for using iwth the media module

Convert media tags to markup
List style CSS classes
Image Alignment CSS Classes