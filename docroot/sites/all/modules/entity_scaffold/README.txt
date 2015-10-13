Entity Scaffold
---------------

Entity Scaffold is intended to be an easy way of creating your own Entities 
by auto-generating a basic Entity which the developer can customize later.

It is mainly based on Drush, but also have an UI in the administrative area
where, in both cases, the developer may pass an Entity name, the name of the
module which the Entity will be part of.

Then, Entity Scaffold will create all the needed hooks, schema, etc and place
the new Entity files inside the given module's tree.
