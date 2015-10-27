Metatag Taxonomy Facets
===============

Description
-----------

Metatag Taxonomy Facets module provides metatags functionality for pages based
on active taxonomy terms facets. It should be considered as a submodule of
Metatag module
(http://drupal.org/project/metatag).

Module creates special entities to store metatags information on taxonomy facets
pages. It provides terms' tokens support for templates on facets combinations
and handles metatags for any given terms combinations. Provided tokens namespace
 is "taxonomy_facets" so in templates they looks like
[taxonomy_facets:field_you_field:name].

It currently doesn't support cases when multiple terms are active in any facet.

Note: this module's metatags are based entirely on active facets and disregard
url's structure.

You can alter meta tags specified by this module on any page using
hook_metatag_taxonomy_facets_rendered_metatags_alter($metatags)
Furthermore you can disable this module's meta tags by setting $metatags to
empty array inside this hook.

Dependencies
------------

Metatag
Entity
Facet API


Configuration
-------------

- Enable and configure your taxonomy terms' facets on the search_api settings
  pages.
- Select facets of your searchers here:
  admin/config/search/metatags/taxonomy_facets
- Next step you can enable metatags templates for any combinations of selected
  facets here:
  admin/config/search/metatags/config/add
  When template for the combination is set up, local task link "Metatags"
  appears on every page corresponding to those selected facets, where you can
  tune metatags for any particular terms' combination.
