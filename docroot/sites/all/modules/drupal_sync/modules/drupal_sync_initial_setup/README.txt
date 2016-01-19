Description
-----------
This module is an extension of the basic drupal_sync module.
It allows connecting websites that already have content.
After establishing connections content between websites will be synchronized when
making changes in connected content.
"Node" entities connections are made accordingly to “title” fields matching.
"Taxonomy_term" entities connections are made accordingly to “name” fields matching.

Usage
-----------
1)Enable the module on all sites that should be connected.
2)Choose entities that should be connected on module configuration page
“admin/config/drupal_sync/drupal_sync_initial_setup” on all sites.
Note. All chosen entities should be set for synchronization in basic drupal_sync module.
3)Start up connection process by clicking "Run initial setup for selected nodes and vocabularies"
link on the same configuration page of one of the sites.
Setup process can be run several times. All already connected entities will not
be connected second time.