CONTENTS OF THIS FILE
---------------------

 * Description
 * Installation
 * Configuration
 * Usage
 * Support

DESCRIPTION
-----------
Active Taxonomy Links module processes nodes of selected content types replacing
words in the nodes with links to taxonomy terms. 

This module is not a copy of http://drupal.org/project/alinks, it is totally 
different. Because current module rewrites node body (Alinks does not change
the node body as it is executed on display only). So Active Taxonomy Links
module doesn't affect site perfomance at all, because all the changes are done
before the node gets viewed by user. So even if you have lots of taxonomy
(vocabularies and terms) your site perfomance won't suffer. Active Taxonomy
Links module prevents server loading huge amount of content.

INSTALLATION
------------
Installation:
 * Installation is as simple as installing the module.

CONFIGURATION
-------------
 * admin/config/content/activetl . Configure module settings:
   
   - Choose content types you'd like to process via Active taxonomy links module

   - Choose vocabularies to get terms to replace

   - Choose limit number of matchups that will be converted to links in field

   - Choose case-insensitivity for first character

USAGE
-------------

 * Module automatically processes new added or created nodes.

 * To protect a single word or area from taxonomy link converting, you should
   wrap it to special HTML tag <no-activetl>WORD</no-activetl>. 

 * The quantity of nodes from content type (not processed yet)

   - admin/config/content/activetl/process - Batch Process: You can select
   content types you want to process. 

 * The quantity of fields in nodes from content type (already processed)

   - admin/config/content/activetl/unprocess - Batch Unprocess: You can select
   content types you want to unprocess

 * Users are able to delete the module results.

SUPPORT
-------
Feel free to report bugs and propositions in our Issue Queue
http://drupal.org/project/issues/active_taxonomy_links


-------------------------------------------------------------------------------
Active Taxonomy Links for Drupal 7.x
	by ADCI, LLC team - www.adcillc.com
-------------------------------------------------------------------------------

