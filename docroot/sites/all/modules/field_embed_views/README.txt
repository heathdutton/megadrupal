Field Embed Views
=================

The module provides a field type to attach any Views View display with, 
configurable on per-entity level, default values for exposed filters 
and sorting. The placement of the View in the content can be reordered 
on the 'Field Display' administration page for the entity.

Usage
=================

1. Go to 'Admin -> Modules' and enable Field Embed Views module.

2. Then go to 'Admin -> Structure-> Views -> Add new view' 
   and define a new View or use already create one. 
   
3. Configure any exposed filters.

4. Add the corresponding field to any entity, e.g. to a node. 
   Use the 'Manage fields' interface provided by the 'Field UI' module 
   of Drupal, located in 'Admin -> Structure-> Content types -> 
   Article -> Manage fields'.
   In the field instance settings select a View and a display.

Known issues
=================

1. As for now, the Field Embed Views modules saves only exposed filter settings.

If you find a problem, incorrect comment, obsolete or improper code or such,
please let us know by creating a new issue.
