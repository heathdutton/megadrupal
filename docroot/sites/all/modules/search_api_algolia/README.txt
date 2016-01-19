CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Known problems
 * Maintainers
 * Dependencies


INTRODUCTION
------------
This module provides integration with the Algolia service
(https://www.algolia.com), through Drupal's Search API. This module is intended
to be used by developers, as it does not currently provide any implementation of
an actual search interface. Only indexing is currently supported. As a result,
enabling that module will not have any visible effect on your application.
Search functionality may be implemented using the Algolia Javascript API.

Currently supported:
 * initial indexing of node entities (see "Know problems")
 * re-indexing on node updates or through forced re-index action

Type of fields which have been successfully tested:
 * entity reference fields (either single or multivalued)
 * standard text fields containing either strings or integers (integer support 
   is important for comparison functions in the search query)


REQUIREMENTS
------------
This module requires the following modules:
 * Libraries API (https://www.drupal.org/project/libraries)
 * Search API (https://www.drupal.org/project/search_api)
 * Entity API (https://www.drupal.org/project/entity)

This module also uses the following library:
 * Algolia search client PHP library 
   https://github.com/algolia/algoliasearch-client-php


INSTALLATION
------------
Please refer to the INSTALL.txt file.


KNOWN PROBLEMS
--------------
 * At the moment, the HTML filter (Index > Filters > Processors) with Tag boosts
   is not supported as it creates separate entries in the items tree. Please 
   make sure that the "Tag boosts" text area is empty.
 * Only node entities are supported for now. User entities have been 
   successfully implemented but more tests need to be completed.
 * Standard multi-valued select fields are not indexed properly as both key and
   value are shown in the resulting indexed data.
 * "Clear all indexed data" button on the index page is not implemented yet, as 
   it collided with normal indexing operations. To clear the index, please 
   proceed from the Algolia dashboard.


MAINTAINERS
-----------
Current maintainer: Matthieu Bergel (mbrgl) - https://www.drupal.org/user/489214

The initial development of this module has been sponsored by TroisCube 
(http://www.troiscube.com).


DEPENDENCIES
------------
This module requires an account on https://www.algolia.com, where you will be
able to generate the required Application ID and API key. A 14-day free trial
period is granted with every new account.

Pricing options can be found on the Algolia website: 
https://www.algolia.com/pricing
