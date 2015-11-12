
ENTITY BUNDLE REDIRECT MODULE FOR DRUPAL 7.x
--------------------------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Dependencies
   * Installation
   * Changelog
   * Support
   * Credits


DESCRIPTION
-----------

This module allows to create 301 redirections for unused entity paths,
by bundle and language.

For example, if you have some content type, and you don't want people
to visit it's corresponding "node/%node" page (because that content
type is not a page-like content type, it's just an object-like content
type that must remain hidden).

Another useful case, is when you desire to redirect Taxonomy term page to
an existing View page, with a given exposed filter selected ($_GET parameter).

The configurable redirections are compatible with tokens, and exportable
via Features.

A 'entity_bundle_redirect_alter' hook is available, so you can modify
the redirection "on the fly" for each entity.


DEPENDENCIES
------------

This module has no dependencies since 7.x-1.2 version.

Anyway, Token module is recommended in order to take advantage of special
and advanced URL configuration options: 
http://drupal.org/project/token


INSTALLATION
------------

1. Just download and enable the module.
2. Assign 'administer entity_bundle_redirections' permission to desired
   roles. This roles can now configure redirections, so be careful.
3. Go to settings page via admin/config/search/entity-bundle-redirect
   or Configuration -> Search and metadata -> Entity bundle Redirect
   and configure the redirections as desired.


CHANGELOG
---------

7.x-1.2
  Fixed Issue #1988174, by @FiNeX: Empty settings page. 
  Removed Token dependence (but integrated with tokens, if installed).

7.x-1.1
  Language bug concerning caches fixed (not reported in issues queue).
  Cache table is now flushed when flushing all caches.
  Features integration.

7.x-1.0
  First functional version.


SUPPORT
-------

Donation is possible by contacting me via grisendo@gmail.com


CREDITS
-------

7.x-1.x Developed and maintained by grisendo
