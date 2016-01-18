
 -- SUMMARY --
 
 The merge comments module provides the option of automatically merging two consecutive 
 comments made by the same user in the same node. If enabled this can eliminate accidental
 double posts as well as serve as a last ditch measure against flooding.
 
 You can decide under which conditions the comments will be merged, what format the newly
 merged comment will have and which users can disable the merge comments module from 
 merging a specific comment.
 
 Additionally this module will take into account your comment hierarchy. 
 So, if you have threaded comments enabled, they will be merged only if they have the same 
 parent, or if a user is replying to his own comment.
 
 
 -- REQUIREMENTS --
 
 Comment module.
 
 
 -- INSTALLATION --

 Install as usual, see http://drupal.org/node/70151 for further information.

 
 -- CONFIGURATION --
 
 * Configure user permissions in People >> Permissions >> Merge Comments
   
  - Administer merge_comments 

    Users in roles with the "Administer merge_comments" permission will be able to set the
    site-wide merge comments configuration options.
    
  - Disable merge_comments 

    Users in roles with the "Disable merge_comments" permission will receive the option
    of disabling the merge comments feature on any specific comment they are writing.
    
 * Customize the menu settings in Configurations >> System >> Configure merge comments
 
 Note: To use the advanced time form or a custom date format you must tick the "Use the advanced form" 
 or the "Use a custom date format" checkboxes respectively, otherwise the defaults are used.


