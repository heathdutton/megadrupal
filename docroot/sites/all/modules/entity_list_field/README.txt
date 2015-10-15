Entity List Field
-----------------
by Ryan Walker, http://drupal.org/user/1791688, http://webcommunicate.net


-- SUMMARY --

Entity List Field provides a field for configuring and displaying entity lists. Add an Entity List field to any fieldable entity to supply your content editors with a simple UI for configuring lists.

This module may be appropriate when you want content editors to be able to configure an entity list from within the familiar add/edit form for the container entity.


-- DEPENDENCIES --

* Entity API (entity)
* Entity Lister (entity_lister)


-- USE CASES --

#1 -- List nodes from blocks (e.g. for a list of recently updated articles). Create the block type (bean) and add an Entity List field to your bean in the Manage Fields tab. Configure the field instance as appropriate, hiding or exposing the configuration inputs to suit your editors' particular needs. Your content editors can now add lists to the blocks they create, and can configure the lists separately for each block.

#2 -- List users from a node (e.g. for a staff page). Create a content type and add an Entity List field in the Manage Fields tab. Configure the field instance as appropriate, then position the field as needed relative to other fields. Your content editors can now add lists of users to the nodes, and can configure the lists separately for each node.


-- HOW TO USE THIS MODULE --

* Download and enable the module.
* Go to the Manage Fields tab for any fieldable entity and add an Entity List field.
* Configure the field.
* Create or edit an item that has an Entity List field.
* To prevent a list from appearing, set the Item count input to 0.
