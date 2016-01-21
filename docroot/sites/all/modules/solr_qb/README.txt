CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration

INTRODUCTION
------------
This module provides UI for building and sending custom queries to Apache Solr.
In some situations you may not have access to the Solr admin interface and Solr Query Builder can help you send and debug your queries in this case.

This project contains drivers for the following modules:
 - Apachesolr (https://www.drupal.org/project/apachesolr)
 - Search API (https://www.drupal.org/project/search_api)
 - Acquia Search (https://www.drupal.org/project/acquia_connector)
 - Acquia Search for Search API (https://www.drupal.org/project/search_api_acquia)

 * For a full description of the module, visit the project page:
   https://drupal.org/project/solr_qb


 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/solr_qb

REQUIREMENTS
------------
Solr Query Builder does not have any requirements, but additional driver modules have requirements related to integrated modules.

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * (optional) Enable additional driver modules.
 * Select and configure the driver at admin/config/development/solr_qb/settings
