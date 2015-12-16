Views GSA Feed
=================

Overview
--------
This module allows the user to make an XML feed for the Google Search Appliance 
(GSA) to be able to index. This module is solely for an external Google Search 
Appliance as it does not attempt to connect to the appliance itself.


Features
--------
Since Views GSA Feed is a Views plugin simple configuration allows the feed to:
  Set a datasource
  Include any fields
  Filter the feed
  Paginate the feed
  Preview the feed
  Ensure tags meet XML requirements


Requirements
------------
Drupal 7.x
Views


Installation
------------
Install and enable the Views module.
Install and enable the Views GSA Feed module.
Installation is complete! View documentation (below) to use.


Documentation
-------------
How to use Views GSA Feed - http://drupal.org/node/1939912

Google Search Appliance Documentation:
  Main documentation page - 
    https://developers.google.com/search-appliance/documentation/68/
  Feeds Protocol Developer's Guide - 
    https://developers.google.com/search-appliance/documentation/50/feedsguide
  External Metadata Indexing Guide - 
    https://developers.google.com/search-appliance/documentation/50/metadata

Known Problems & Functionality Not Yet Implemented
--------------------------------------------------
Does not support GSA content feeds.
Does not support incremental GSA feeds.
Does not include deletes in the GSA feed.


Credit
-------
This module is heavily based off of the Views Data Export module.
