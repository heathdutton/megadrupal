
-- SUMMARY --

Drupal to Simple Machines Forum 2 (SMF2) posting module.

 
-- REQUIREMENTS --

 - SMF 2.0 "REST" API from https://github.com/erazorbg/smfapi used with MODIFICATIONS, so you shouldn't use it.
   If newer API appear on github, API files that included on module will updated after testing.

 - CURL (php library) should be enabled (SMF2 API REST requirements).
 
 - Token module: https://www.drupal.org/project/token.


-- INSTALLATION --

 - Install as usual, see https://www.drupal.org/documentation/install/modules-themes/modules-7 for further information.

 - Copy server part smf2p\inc\api to SMF2 site.
   So path should like: http://YourSMF2Site.com/api
 - Open there /api/index.php file, row #48 and change SECRET_KEY to any row, for example:
   define ('SECRET_KEY', 'Put your secret key here');


-- CONFIGURATION --

 - Visit "SMF2 Posting settings" - admin/config/content/smf2p - for configuring:
  * Fill secret key, that you enter at "Installation" time.
  * Fill Absolute URL to SMF2 site: http://YourSMF2Site.com
  * Fill Absolute URL to copied SMF2 API REST site folder: http://YourSMF2Site.com/api
  * Enter Email of user, that will author of posts in SMF2 site.
  * Click "Save configuration", and if all ok, new settings fields appear. Otherwise, check error messages.
  * Fill next on success:
    - Allowed node types;
    - Display link to forum at node view as field or in links
    - Title template;
    - Body template;
    - For title and body use tokens tree for finding correct token replacer, especially under "Nodes".
    - Some tokens generated only after node save submitting, so you change these [TOKEN:VALUE] to {{TOKEN:VALUE}},
      for example: Node NID generated only after submitting, so you should do next: [node:nid] to {{node:nid}}
      But title of node should be [node:title], since it used from filled form elements.

 - Visit "Permissions" - admin/people/permissions for configuring "smf2p" permissions:
   * Publish CONTENTTYPE to SMF2 - enable for selected roles.

 - Visit selected content types field display admin page:
  * For example, Page content type: admin/structure/types/manage/page/display
  * Check that "URL to Forum topic" is visible and correctly ordered.


-- USING --

For posting node to forum:
 * Add node with allowed node type and appropriate user permission.
 * Fill required node form fields. Passing validation of form is important, since enabling posting 
   to forum will check it.
 * Click Community fieldset options at the bottom and check Create post on forum.
 * Title and Body will regenerated with current filled token. If you will change node form elements above, 
   you should click Refresh for updating current fields.
 * Select certain Board.
 * Check or change title and body. Please don't touch {{TOKENS}}, this is unaccessible tokens at this 
   moment (for example, NID will generated only after submitting).
 * Click Save.
 * Check Node view, forum topic link should appear. Try click, it open new window with generated topic.

For editing posted forum settings on node:
 * Edit node, that already have posted to forum.
 * Now you cann't edit title and body.
 * You can change only URL of forum (so you should prepare forum topic on SMF2 site).
 * You can also uncheck "Create post on forum", if you don't want show Forum link. 
   If you will enable it again, you still can change only URL.

-- NOTES -- 

 - Warning: If you move SMF2 forum with "api" folder to other place, please remove dynamically 
   created "smfapi_settings.txt" file folder on "api" folder.


-- TODO --
 * Make posted forum topic to be changeable after posting. So title, body and board can be changed at node editing.
 * Make check that user can post to selected board.

-- PATCHES --
Temporary: 
For supporting cyrillic: SMF2_site/Sources/Subs-Db-mysql.php #374 add: 
@mysql_query("SET NAMES utf8");
I don't know why they made this for PostgreSql, but don't do for others.


-- CONTACT --
http://nikiit.ru/contact
