README.txt
==========

-- SUMMARY --

This is a simple but powerful module for Drupal 7 that allows administrators
to effectively manage child entities when their parent entities are deleted.
Upon parent entity deletion, it gives the option to either delete all
dependent entities or drill down the tree of child entities and unselect any
that you wish to maintain. The term "parent" and "child" depends on the set
up. Some may prefer to set up their children to refer to their parents, and
others may prefer to set up their parents to refer to their children. Only
users who have the correct permissions will be able to access this option;
otherwise, no dependent entities will be deleted.

Currently, this module only supports entities that are in Core and the
Rules:config entity. There appears to be an issue with Comment entity
references, so this has not been tested yet. In addition, there is no
explicit way to delete file entities in Core and, therefore, there is no
delete confirm page and not supported in this module.

-- REQUIREMENTS --

Entity Reference: http://www.drupal.org/project/entityreference


-- INSTALLATION --

Install as usual. See Installing contributed modules (Drupal 7) for further
information:
http://www%2Cdrupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

1. Go to Entity Reference Cascade Delete settings (admin/config/content/ercd)
to choose the correct entity reference model. Then choose which entities can
trigger a cascade deletion when they are deleted and which entity types will
be subject to the cascade deletion. Keep in mind that users will have the
option of unselecting all specific entities on the "confirm delete" pages.
2. Set appropriate permissions for each role. Permissions are broken down to
each parent-child entity reference.

-- TROUBLESHOOTING --

If you don't see any referencing/referenced entities showing up--or not as
many as you expected to see--when deleting an entity, check the following
things:
1. Make sure you have actually enabled a cascade deletion for that entity.
2. Make sure your user role has permission to perform the cascade deletion.
3. Check if the entites you were expecting to see are referenced/referencing
an entity that is outside the scope of the cascade.


-- CONTACT --

Current maintainers:
* Andrew Howell (atozstudio) - http://drupal.org/user/572698
