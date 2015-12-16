Views Entity Access
===================

Introduction
------------

This module is designed to provide a way for a view to expose in the permission 
control settings an option to use Entity Types or Entity Bundles as limitations.

Using the "Views Entity Access" module
--------------------------------------

1. Add a new "Page" display to your view (or any view which has access controls)
2. From the Page Settings section, select "Entity Access check" from 
   Access options.
3. On the settings you'll be provided options to choose from known "Permissions"
   and from "Entity Types".
   
Note: At this time, the Entity Types lists all the Entity and Entity Bundles. 

To Do
-----

1. Improve the Entity Types selector

History
-------

This module was created from the need to limit a local menu tab to be only 
available on a particular entity and not everywhere in the system. Instead of 
being dependent on other Access Control modules, this one puts the control 
right into the VIEW itself.