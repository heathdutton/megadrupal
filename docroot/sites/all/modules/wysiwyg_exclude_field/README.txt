Wysiwyg exclude field adds control option on textarea's field type to
allow administrators to choose if the wysiwyg editor should appears on form. 

With wysiwyg module, the only way you have to define a textarea's field to be
loaded without wysiwyg editor is to create a new text format which will
have no editor selected.
This module fill in this lack by providing an option to unload wysiwyg editor on
a selected field. 

Installation
------------
1. Copy Wysiwyg_exclude_field into your modules directory and then enable it
on the admin modules page,
2. Go to the edit instance settings form available on "Manage fields"
(eg: <root>/admin/structure/types/manage/article/fields/body),
3. You should now see a new option called "Unload wysiwyg for this field." which
allows you to select if you want load, or not, the wysiwyg in the field.
