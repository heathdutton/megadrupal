DESCRIPTION
-----------

Hashtags module allow users to create hashtags for their nodes using (#) sign.

FEATURES
------------
 * Hashtags module creates hashtags filter which automatically will be attached to Filter HTML & Full HTML input formats;
 * If you have old version installed - just run /update.php - it will attach Hashtag filter;
 * Regular expression for parsing hashtags has been built according rules from hashtags.org;
 * Hashtags work only Body field - in near future I plan to make it work for any textfield/textarea fields (by admin choise);
 
03-01-2016
----------
Updates:

1) Hashtags saving method changed. Now hashtags will be attached even after creating entity from code:
$node = node_load(12);
$node->field_body['und'][0]['value'] = 'my text #hashtag';
node_save($node);

2) Hashtags storing method changed. Now there's a new table 'hashtags_index' that stores relation between main entity (usually node), hashtag (tid) and comment (cid).
This table is udpated with hook_entity_presave(), hook_entity_insert for main entities and with hook_comment_presave(), hook_comment_insert() for comments. The goal of this table is avoid heavy-performance operations according to parsing and analysing texts of entity and comments to figure out what hashtags were added/removed.

3) Ability to use hashtags in comments. All hashtags that created for entity fields or comments fields will be stored in field_hashtags terms field that attached to main entity. In order to recognize what hashtag is belonged to main entity and what is belonged to comment 'hashtags_index' table will be created. So some hashtag will be removed from field_hashtags only after checking if it's not existied for current entity and any comments of this entity by checking corresponding data from 'hashtag_index' table.
https://www.drupal.org/node/1551532

4) Ability to use hashtags for multiple text fields. Go to Manage Fields > {Your textfield/textarea field} and check 'Track hashtags for this field' box. This box allowed only for bundles that have field_hashtags field is attached.
https://www.drupal.org/node/1998008

5) Ability to create hashtags for different entities like users, files etc. not only nodes. Now there's an UI for easily activating hashtags for Content types only. But in near future it will be changed to be easily activated for any bundles. For now if you want to activate hashtags for another type of entity (User) you need to make it manually. Go to Manage fields page of your entity (admin/config/people/accounts/fields) and make 2 things:
*) In 'Add existing field' option choose Field Hashtags and add it.
*) Edit your textfield/textarea field and check 'Track hashtags for this field' box.

6) Bug with linking of first hashtag has been fixed.
https://www.drupal.org/node/2430127

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
    
    