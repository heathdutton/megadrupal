Taxonomy Orphanage
==================

This module finds and removes orphaned taxonomy references on entities. It is needed because core Drupal [does not yet do this](http://drupal.org/node/1281114#comment-5678238).

## Config

Navigate to Admin -> Structure -> Taxonomy -> Orphanage.

You can have the roundup run during cron, but note that if your crons are run by the web server and not by drush you'll likely want to keep the number of entities you process to a minimum in order to avoid timeouts.
