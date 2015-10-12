# Civi-jQuery

CiviCRM and Drupal both place a copy of jQuery on the page, which is inefficient. This module removes Drupal's (older) copy and uses the single copy from CiviCRM for both applications. This results in faster page loads and a more responsive web browser.

- The effect on Drupal is similar to installing the [jQuery Update](http://drupal.org/project/jquery_update) module.
- There should be no effect on CiviCRM.

### Fair warning

Drupal 7 core & contrib modules have had quite a long time to get used to the idea of jQuery versions released after 2010, but some still haven't caught up. Any module or theme incompatible with jQuery Update will not be compatible with this module either.

### Development

- The issue tracker is on [Drupal.org](https://www.drupal.org/project/civi_jquery).
- Development is done on [GitHub](https://github.com/colemanw/civi_jquery).
