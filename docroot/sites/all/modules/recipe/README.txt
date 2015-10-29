// $Id:

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Install
 * TODO
 * Database Information

INTRODUCTION
------------

Current Maintainers: brdwor, drawk, marble, tzoscott and jvandervort.
Original Author: Moshe Weitzman <weitzman@tejasa.com>

Recipe is a module for sharing cooking recipes.


INSTALL
-------
See INSTALL file for important instructions.


TODO
-----
NOTE: Some of these TODO items are old and predate the Drupal 6 architecture.
This means that some of these TODO items might be possible now, but have not
been tested or confirmed using other modules together with Recipe..
Some help testing/experimenting is appreciated.

- Make recipe_unit more translatable. This might mean moving units to an
  included array file, or integration with one of the Drupal 'unit' modules.
- Get ingredients into the searchable Index. Requires some SQL expertise.
  See recipe_update_index()
- Let users maintain their own recipe collection just like a blog or
  personal image gallery
- Integrate with bookmarks.module so users may create a 'recipe box' listing
  the favorite recipes
- Views2 support - Views enabling, and custom handler for 'many' fields
  (such as Ingredients).
- Investigate CCK Multigroup and Fields for D7.


DATABASE DESCRIPTION
--------------------

Data is saved in normal form. Recipes are collections of pointers to
ingredients and to quantity terms. New terms can be added by modifying the
schema. New ingredients are added automatically whenever they are used for
the first time.

Following is an ASCII art attempt to illustrate the DB relationships:

node.nid +--------------+     +------------------+     +-------------+     +--------------+
     ^   | recipe       |     | _node_ingredient |     | _ingredient |     | _unit        |
     |   +--------------+     +------------------+     +-------------+     +--------------+
     +---| nid          |<--  | id               |   +-| id          |  +--| id           |
         | source       |  +--| nid              |   | | name        |  |  | name         |
         | yield        |     | unit_id          |<-+| | link        |  |  | abbreviation |
         | instructions |     | quantity         |  || +-------------+  |  | metric       |
         | notes        |     | ingredient_id    |<-|+                  |  | type         |
         | preptime     |     +------------------+  +-------------------+  +--------------+
         +--------------+

