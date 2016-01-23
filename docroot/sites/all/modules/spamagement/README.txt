===========================================================
OVERVIEW
===========================================================
Tools for managing spam that gets past other filters. 
In other words “Perform bulk operation to delete spam 
content/comments/users”.

===========================================================
DEPENDENCIES
===========================================================
- views_bulk_operations 
- admin_views
- views_ui

===========================================================
INSTALLATION
===========================================================
STEP1- Download the module and dependencies module.
STEP2- Enable the module and dependencies module.
STEP3- Go to yousite.com/admin/structure/views/view/admin_views_node/edit.
STEP4- Click on the fields "Bulk operations: Content" .
STEP5- FROM "Provide a checkbox to select the row for bulk operations.".
     - Check "Report and delete all of a spam user's content, as well as 
     the user itself."
     - "Note - you can also override the label "
STEP6 - Save the views and now you are ready to perform action.

===========================================================
Do a Test
===========================================================

STEP1- Go to admin/content and choose the content which you see as a 
       spam content.
STEP2- Select the action "Report and delete all of a spam user's content, 
     as well as the user itself." and click the execute button.
STEP3- You will see the node has been added in a batch processing.
After this you will see the content and its user has been removed.

Note : Module will not delete the content if the content has been 
created by userid=1 (See the issue https://www.drupal.org/node/2546492)
