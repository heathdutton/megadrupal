Content Admin Tree
=====================

This is a module which changes the backend
experience of content administration.
It creates a navigation tree for administering content
with dynamic views for each page.
You can navigate through content types and the taxonomy
terms attached to each content type in tree form if any
are attached.

The structure is set out like this:

-- Content type 1
----- Tax Term Parent 1
-------- Tax Term Child 2
----- Tax Term 3

-- Content type 2

Instructions
====================

 - Firstly make sure the following modules are installed:
   - Views [http://drupal.org/project/views]
   - Views bulk operations [http://drupal.org/project/views_bulk_operations]
   - Ctools [http://drupal.org/project/ctools]
   - Search API [http://drupal.org/project/search_api]
 
 - Now install the module.
 
 - Now if you haven't already, create some content types and attach
   vocabularies. There is also a configuration form at
   admin/config/content/content_admin_tree
   which allows you to select which content types you want to display.
   
This module is made for drupal 7.
