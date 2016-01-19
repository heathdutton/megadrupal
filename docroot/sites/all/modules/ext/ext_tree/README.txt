OVERVIEW

Allows an administrator to create Ext Models and Stores for use in Ext Trees.

Ext tree node Models are based on an existing Model, referred to as the base
Model. All the fields and properties from the base Model are inherited, and 
node tree specific fields are added. For a list of tree node fields see  the 
config options for the Ext.data.NodeInterface:
http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.NodeInterface

In order to use a Model as the base it must contain an association that links 
to the Model (ie be self-referencing) in order to construct a tree hierarchy.

The user selects the association field to use for the child/parent 
relationship that will be used to construct the tree hierarchy.
The user can also select which fields from the base Model to map to tree node 
properties/fields (eg text, icon, href). It is recommended that at least the 
'text' option be specified. 

When a base Model field is mapped to a tree node field then a change in the 
value of one field will be automatically copied to the mapped field (in both 
directions).

A user with the 'administer site' permission is able to create new tree node 
Models by going to Administer > Site Building > Ext > Models > Add Model, 
entering a name for the new Model and selecting the base Model. Then the 
tree node Model can be configured.


REQUIREMENTS

The ext module.
