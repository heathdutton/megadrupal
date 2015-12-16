DESCRIPTION
-----------

Hashtags module allow users to create hashtags for their nodes using (#) sign.

FEATURES
------------
 * Hashtags module creates hashtags filter which automatically will be attached to Filter HTML & Full HTML input formats;
 * If you have old version installed - just run /update.php - it will attach Hashtag filter;
 * Regular expression for parsing hashtags has been built according rules from hashtags.org;
 * Hashtags work only Body field - in near future I plan to make it work for any textfield/textarea fields (by admin choise);
 
REQUIREMENTS
------------

 * Taxonomy module
 
INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "hashtags" in the sites/all/modules directory and
    place the entire contents of this hashtags folder in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page:
      Administer > Site building > Modules (admin/modules)

 3. HOW TO ADD HASHTAGS
    While you add/edit a node you can put # sign before any word. After
    submitting a form all #-words turn into terms. Hashtags will be 
   stored in default vocabulary Tags (you can add origin tags as well (without #) or go to config page and hide field.

 4. CONFIGURE HASHTAGS

    Go to hashtags configuration page: (admin/config/content/hashtags);
    
    