MEAN Shadow
===========

The MEAN Shadow module provides the ability to shadow entities from drupal to the MEAN stack.
You can choose through the settings pages which entities will be automatically shadowed on insert/update'
'delete' is not yet supported.

Shadowing entities allows you to have more freedom when you want to use the drupal entities directly from the MEAN stack.
A good use case for that is a graph or statistics page where you don't want to constantly use ajax requests to get data from the drupal server side.

In addition you can manually use drush to batch shadow the existing entities to the MEAN stack.

* Feel free to use drush shanties instead of the long drush shadow-entities, it's MEANer :)

Examples:
 Standard example (all the user entities)  drush shadow-entities
 Type example (all the node entities)      drush shadow-entities node
 Id example (specific user)                drush shadow-entities user 42
 Id example 2 (specific node)              drush shadow-entities node 42
 Bundle example (all the entites of        drush shadow-entities node article
 bundle article)

Arguments:
 entity type                               An optional type to shadow, user (default) or node
 bundle type                               An optional bundle (node type)
 id                                        An optional id to shadow


Installation
============
1. Install & enable the module.

2. Make sure to configure the integration settings on 'admin/settings/mean/settings'.

3. Make sure to select which entities will be automatically shadowed for you on 'admin/settings/mean/shadow'.

3. Use the available drush command to batch shadow everything you want.

Contributors
============
- basik.drupal (Ehud Shahak)