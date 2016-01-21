CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------

Chinese, Japanese and Korean words are not separated by space. If we can invoke
Solr (with CJK word-segmentation tools configured) to pre-segment the search
keywords,  and then re-invoke the Solr to search the segmented words, the
search result will be much better.  This module applies to this situation.

Requirements
------------
 * Search API Solr Search (https://www.drupal.org/project/search_api_solr)

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * Chinese word segmentter configuration for Apache Solr can refer to:
   http://www.dplor.com/node/309

CONFIGURATION
-------------
No configuration via UI, just enable the module, it will work.
If you need not this feature, just disable this module.

MAINTAINERS
-----------

Current maintainers:
 * Vincent Zhang (fishfree) - https://drupal.org/u/fishfree
 * Zhang Terry (zterry95) - https://www.drupal.org/u/zterry95
