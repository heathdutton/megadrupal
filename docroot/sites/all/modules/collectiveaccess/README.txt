Description:
============

This module provides the ability to retrieve data from your CollectiveAccess collection management system (http://www.collectiveaccess.org).

Functionality overview:
-----------------------
* CollectiveAccess module
  - UI to manage connections to one or more CollectiveAccess instances
  - Ability to connect to the web services via several technologies
    (only connection via SOAP is implemented at this point)
  - Provides API for retrieving CollectiveAccess data programmatically
    x Object basic data and attributes
    x Object representations
    x Related entities to an object

  This module currently only works with CollectiveAccess branch "cleanup_before_release", from revision 7618 onwards.
  This module is *not* yet compatible with the available packaged downloadable Providence 1.0.

* CollectiveAccess Feeds module
  Allow for automatic retrieval, updating and field mapping of CollectiveAccess data to Drupal entities (nodes, Commerce products, ...)
  As per Feeds functionality periodic updates can be set up.

  Available Feeds plugins:
  - FeedsCollectiveAccessFetcher: allows Feeds to connect to one of the CollectiveAccess instances available on the website
  - FeedsCollectiveAccessParser: parses CollectiveAccess object data for usage by a Feeds processor
    This allows you to create nodes, Drupal Commerce products, etc... out of the retrieved data

  Support for mapping to multilingual fields is available, but requires a patch to Feeds module:
  http://drupal.org/node/1183440#comment-4647432

Roadmap:
---------
Coming up:
  - support for more relations
  - support for non-object centric functionality

Someday:
  - Add a connector for REST, JSON, ...


Information for developers:
===========================
- extra web services connectors can be added by extending the CollectiveAccessConnector class
- extend other classes in order to customise the behaviour to your needs