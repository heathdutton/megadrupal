Entity Lister
-----------------
by Ryan Walker, http://drupal.org/user/1791688, http://webcommunicate.net


-- SUMMARY --

Entity Lister provides a class (EntityLister) for querying and theming entities.
Entities are queried using EntityFieldQuery and themed with view functions (e.g.
node_view, user_view, etc.).

This module is for developers only. It provides no GUI, except for the
pager and cache viewer.  To use the module, enable it and start instantiating
EntityLister objects.


-- DEPENDENCIES --

Entity API (entity)


-- DEVELOPER RESOURCES --

  * For usage examples please refer to entity_lister_demo() in
    entity_lister.pages.inc.  When you install this module, the demo page is at
    <yoursite>/entity_lister/demo.

  * For documentation of config vars, refer to the code comments above the
    constructor in the EntityLister class.  For default values, refer to
    EntityLister::defaults().

  * When and how to use a custom pager with entity_lister:
    https://drupal.org/node/2015225

  * The cache viewer lists all cache entries created by the module:
    <yoursite>/admin/reports/entity_lister/cache
    You can also find the cache viewer on the reports page:
    <yoursite>/admin/reports


-- FEATURES --

  * Conforms strictly to Drupal coding standards.

  * Configure the number of items, bundles, view mode, multiple levels of
    sorting, pager behavior and position, caching, table headers (for tabular
    displays), etc. For a complete list of config options, see the documentation
    in the class.

  * Supports querying all entity types. You can include only one entity type per
    list, but can combine multiple bundles.

  * Provides caching with cache_set and cache_get. Lists are cached per list
    config and per role(s). A user assigned to roles foo and bar will not share
    a cache entry with a user assigned to foo. The cache is invalidated per
    entity type. That is, if a node of any bundle is saved or deleted, all node
    list cache entries are deleted. If a user is saved or deleted, all user list
    cache entries are deleted. And so on.

  * Lists can be configured to include a standard pager or AJAX pager. The pager
    can be positioned above or below the container, or both. The AJAX pager
    calls scrollTop() and animate() to reset the scroll to the top of the
    container.

  * Includes a Node View Permissions integration.

  * Built-in access control for nodes, users and comments. To control access to
    more entity types (or remove the access control for the aforementioned
    types), extend the class and override accessCheck().
