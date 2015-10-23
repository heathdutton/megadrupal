Module to search files on a local folder and send to a Apache Solr integrated with
Apache Tika, to extract metadata and content from the files and index them.

This module was tested with Solr 3.6.x.

Copy every file from solr-conf to APACHE_SOLR/conf/

ps: it's useful to symlink those files, instead of copy

1 - Start Apache Solr
2 - Install module
3 - Define which folders should be mapped and searched
4 - When you go to index the file data for the first time, use the drush command "drush solr-local first", and on the next indexing turns, "drush solr-local update". The module will save the time that it made the last index, so that for the next times, it only searches for new or updated files
5 - Access /files/search to search your data

P.S.: You may need an extra cache clear after enabling the module to get the custom
search page to get into the menu router