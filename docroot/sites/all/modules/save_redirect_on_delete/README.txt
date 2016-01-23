
-- SUMMARY --

   The Save Redirect On Delete module gives the functionality for saving 
   redirects after deletion of entity(supported by pathauto module) having
   some url alias.


-- REQUIREMENTS --

1. Pathauto
2. Redirect

-- CONFIGURATION --

*  Configure user permissions in Administration » People » Permissions

*  Administer Configuration admin/config/search/redirect/delete-patterns

-- DESCRIPTION --

Problem:

 - Created alias for an entity (say a node) and now node/nid is rendered by
   "abc" (alias for that node).
 - When the node is deleted, the alias is deleted as well.
 - In the meantime search engines had indexed that link and you have also
   bookmarked it, Now when they come to the site it shows Page Not found
   which is not good.

Solution:

 - Add a redirect of abc to a pre-defined page for every Entity (say there is a 
   view for every content type) , "abc" will now have an redirect say "events",
   and it will never show a page not found


   The save_redirect module provides support for generating redirects(When a 
   url alias is deleted) automatically, based on appropriate criteria, with a
   central settings path for site administrators.
  
   Implementations are provided for core entity types: content, taxonomy terms,
   and users (including blogs and tracker pages).
