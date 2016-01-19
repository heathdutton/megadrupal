
PROBABILISTIC WEIGHT MODULE FOR DRUPAL 7.x
------------------------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Requirements
   * Installation
   * Support
   * Credits


DESCRIPTION
-----------

This module provides a field type in order to assign a
"probability weight" to it.

A probability weight is a float number which indicates a weight...
not a fixed weight as many other modules do, but a weight with certain
degree of randomness in order to ensure desired content appears in the
first places with some probability, but not always. This module allows
Views module to sort content in that way.

It's very useful in marketing context, where some products are desired
to be promoted, but not always, in order to avoid being annoying.

Many fields can be assigned to any entity type, so you can have not
only a unique probability weight per content, but many as desired.

Other similar project is Random Weight
(http://drupal.org/project/random_weight),
but there are many differences because the scope is totally different:
  * The most important difference: there is no randomness
    control with Random Weight. Weights are assigned randomly.
    With Probabilistic Weight, you can define which entities are most
    likely to be in the top by assigning them a probability.
  * Random Weight generates a weight randomly using cron. But then, the
    weight is always the same and the entities will be sort the same way,
    until the next cron runs. With Probabilistic Weight, query results
    will be different each time query is executed.
  * With Probabilistic can have more than one probabilistic sort criteria
    per entity (more than one field per entity).

This module intends to generate queries with different query results each
time. If you are using core page caching or Boost
(http://drupal.org/project/boost),
you can use also Ajax Blocks (http://drupal.org/project/ajaxblocks) to
avoid having always the same HTML result for anonymous users.


REQUIREMENTS
------------

Drupal installed on MySQL, SQLite or PostgreSQL database.
Views 3.x

INSTALLATION
------------

Just download and enable the module and a new field type called
"Probabilitic weight" will be available in "Manage fields" form
on any Entity.

Then, entity real sorting is made via Views, adding it as a sort field.


SUPPORT
-------

Donation is possible by contacting me via grisendo@gmail.com


CREDITS
-------

7.x-1.x Developed and maintained by grisendo
