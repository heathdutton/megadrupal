Apache Solr search attachments extra
____________________________________

This module is intended for use when a user wants to only show attachments 
of a certain bundle and not the actual parent entities themselves. Please 
note that this module does not stop the parent entities being indexed by
Solr; it merely filters bundles from the search results.

The next branch of the module will hopefully allow optional indexing of 
chosen bundles but this was initially put together as part of a project
and for development speed, query filtering was the chosen method.

Installation
------------

Install as normal (see http://drupal.org/documentation/install/modules-themes).
Once installed and enabled no configuration is needed; a new option will simply
be added to the attachments bundle config form at 
yoursite.com/admin/config/search/apachesolr/attachments/entity_bundle.

Development
-----------
The development of Promote Disable is sponsored by Eon Creative Limited,
a web design and development company specialising in Drupal development based 
in Glasgow, Scotand.
http://www.eoncreative.com/
