
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Example


INTRODUCTION
------------

Current Maintainer: Evan Willhite drupal.org uid: evanmwillhite

The States module allows administrators to define states that can then be used
in contexts. Also, sets a permission to enable site editors to change these sta
tes in the UI.


INSTALLATION
------------

States can be installed like any other Drupal module -- place it in
the modules directory for your site and enable it (and its requirement,
Context) on the `admin/modules` page.

You will probably also want to install Context UI which provides a way for
you to edit contexts through the Drupal admin interface.


EXAMPLE
-------------

You are setting up an event application and you want to allow site editors to
mark the event as "Sold Out", which then uses context to hide event marketing
content and show event info content based on the Sold Out state.

1. Add the "Sold Out" state in admin/structure/states/define (you can define
   multiple states as well).
2. In your context, select the "States" condition and the "Sold Out" state.
   Set your reactions accordingly, e.g. 1. use "Blocks" to show content based on
   the state, 2. use "Regions" to hide regions based on the state, etc.
3. Users with the "Edit States" permission can toggle the available states at
   any time at admin/structure/states.
