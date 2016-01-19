Entity Scaffold
---------------

Entity Scaffold is intended to be an easy way of creating your own Entities 
by auto-generating a basic Entity which the developer can customize later.

Then, Entity Scaffold will create all the needed hooks, schema, etc and place
the new Entity files inside the given module's tree.

** Usage **
To generates a module containing a basic structure of a custom entity:

Examples:
drush entity-scaffold my_entity /path/to/generate/new/module

** Arguments **

entity_name:
  The name of the entity that will be generated containing the entity
  implementation.

module_destination: 
  The location where the generated module will be placed.


* Note that the entity name will be the name of the new module.