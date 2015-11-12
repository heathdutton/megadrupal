CONTENTS OF THIS FILE
---------------------

 * Important
 * Introduction
 * Installation
 * Module overview
 * Usage examples

IMPORTANT
---------
As of 18. August you should use 7.x-dev because of http://drupal.org/node/1248822

INTRODUCTION
------------

This set of modules aims to be a permanent datastore for important Drupal data.
Based upon that data admins can configure tables and charts to provide informations
for various people.

The main focus was put on the flexibility of that storage system. It builds upon
the Drupal entity/field concept and can be configured to meet the needs of specific
use cases. A simple example can be found in the datastore_system module.

INSTALLATION
------------

 * This package currently contains four modules, probably you don't need
   all of them. So make sure, that you only enable the needed ones (otherwise
   you may have to download many required modules).

 * Copy the whole datastore directory to your modules directory
   (e.g. /sites/all/modules) and activate the needed modules (see next point)
   modules.

MODULE OVERVIEW
---------------

 * datastore_entity: Defines two entity types: datastore and datastore_domain.
   A datastore is the container for time based data which is described with
   default properties and can be enhanced with fields. Datastore domains are a
   way to categorize datastores and work as bundle field (so you can attach fields
   based upon the main domain).

 * datastore_field: Actually the heart of this package. Provides a field type
   to store timestamp/value pairs. In contrast to default fields it implements
   a own storage procedure for performance issues. With this procedure it is 
   possible to add values to the fields without loading all field values. When
   loading a datastore entity, no field values are loaded. Also the default views 
   integration has been changed, so that views uses the field table and not the
   provided data via entity_load.

 * datastore_system: This contains some samples of how to use the datastore
   package. It provides default logging informations like count of users/nodes
   and distribution of taxonomy terms. Uses rules and the rules_scheduler module
   to retrieve and store data.

 * datastore_visualization: Contains simple visualization tools like a style
   plug-in for views. Depends on views and charts which both are mandatory for
   providing charts as blocks or pages. You can use these provided blocks to
   build a custom dashboard or something near it.
   
USAGE EXAMPLES
--------------

tbd