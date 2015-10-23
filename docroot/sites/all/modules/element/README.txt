###   ELEMENT   ###
-------------------

Element builds upon the entity API to provide developers with a fieldable bundleable entity type that offers a lightweight alternative to nodes. The module was a take on the entity API inspired by a surprising rediscovery of its inbuilt functions. While researching alternatives to this it was discovered that many other modules which use the entity API to create custom entities were adding extra unnecessary custom code to create features that the API already had thought about and provided.

The Entity API provides all of the a mechanism for all of the forms needed for creation and maintenance of fieldable, bundleable exportable entity types. The initial goal of this project is to use an elegant and simplistic implementation of the entity API to add a custom fieldable, bundleable, revisionable and exportable entity to the Drupal user's toolbox.

This module adds a new entity type Element. Elements share many of the features of nodes, but are not meant to function as an alternative to them. Elements are connected to users and are bundleable, fieldable, revisionable and exportable. They have all of the CRUD forms that users need for them, have view modes and are templatable. They are intended to be used where some content type or collection of data doesn't fit logically in the "node" basket.

REQUIREMENTS
------------
This module requires the following modules:
 * Entity API (https://drupal.org/project/entity)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

MAINTAINERS
-----------
Current maintainers:
 * Mike DeWolf (mrmikedewolf) https://www.drupal.org/user/2679073
