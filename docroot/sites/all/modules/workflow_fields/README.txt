********************************************************************
                     D R U P A L     M O D U L E
********************************************************************
Name: Workflow Fields Module
Author: Karim Ratib <karim_ratib at yahoo dot com>
Co-maintainers: HB EMTEC (hbemtec) - Mitja Švab (mitjasvab)
Drupal: 7
********************************************************************
== DESCRIPTION ==

The workflow_fields module extends John VanDyk's workflow module by
allowing node fields to be made invisible or read-only depending on 
the current workflow state. The workflow state form is extended to 
show all node fields with appropriate controls to hide and/or disable
each field for that particular state.

This functionality is useful when a workflow moves among users who 
have different permissions regarding the document flowing through them. 
Which is the case in most every workflow we're encountered.

== INSTALLATION ==

It's just like any other classic Drupal module :
1. Place the entire workflow_fields directory into your Drupal 
   sites/all/modules directory
2. Enable the module by navigating to Modules > Install, checking
   "Workflow Fields" and submitting.


== HOW TO ADMINISTER ==

The module's general admin UI is now available under Config > Workflow >
Workflow Fields settings.

Workflow fields' settings are to be administered for each workflow,
When browsing to your workflow's details (/admin/config/workflow/workflow/xxx),
you'll find, for each workflow state, a new link called "Workflow field"
in the "Actions" column. Click on it to administer fields' visibility
for the desired workflow state.

********************************************************************