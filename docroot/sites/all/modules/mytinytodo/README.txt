
myTinyTodo
===========================



Features
------------

Multiple lists
Task notes
Tags (and tag cloud)
Due dates (input format: y-m-d, m/d/y, d.m.y, m/d, d.m)
Priority (-1, 0, +1, +2)
Different sortings including sort by drag-and-drop
Search
Password protection
Smart syntax improves creation of tasks (usage: /priority/ Task /tags/)
Print-friendly CSS
Style for mobiles devices
Field permissions integration (http://drupal.org/project/field_permissions)
Views integration (http://drupal.org/project/views)



Installing myTinyTodo
-----------------------------

1. Copy the Drupal mytinytodo module to your sites/SITENAME/modules directory
   or sites/all/modules directory.

2. Enable the module at Administer >> Site building >> Modules.

3. Go to content types (admin/structure/types) -> manage fields -> look for "TODO list" in the field selection box


To add more TODO lists per content type, repeat step 3.



Security
------------------------------

You must grant permission to access the content type that contains your todo list field by going to /admin/people/permissions.

For more granular control, you can install the "Field permissions" module (http://drupal.org/project/field_permissions). This will
give you control to determine which roles are allowed to view and/or edit the todo list field.
