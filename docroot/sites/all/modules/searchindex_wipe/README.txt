README.txt
-- SUMMARY --

This module clean up search index built by search module. It is helpful
while deploying large sites whose search index becomes massive.
The reindex button does not clear the search index but rather gradually
replaces existing search data with new data as items are reindexed.


-- REQUIREMENTS --

Needs core search module to be enable on the site.

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.


-- CONFIGURATION --

* To clear the search index, go to Administration » Configuration » Search and 
  metadata » Search settings. Hit on Wipe Index button next to Re-index site 
  button.

-- FAQ --

Q: Why should I use this module?

A: The search index can become massive on large sites, making it difficult 
   to transfer the site to another server. Examples include migrating ISPs 
   or just creating a test site. Yes, we know it would be better to not delete
   the entire search index, but its sheer size sometimes forces the need.


Q: I wiped my search index, How do I rebuilt it?

A: It is similar to Rebuild index button, we need to run cron on the site to 
   rebuilt the index.


-- CONTACT --

Current maintainers:
* Sagar Ramgade (Sagar Ramgade) - http://drupal.org/user/399718
* Nitesh Pawar (Nitesh Pawar) - http://drupal.org/user/1069334
