------------------------------------------------------------------------------
  collapser module Readme
  http://drupal.org/project/collapser
  by David Herminghaus (doitDave) www.david-herminghaus.de
------------------------------------------------------------------------------

Contents:
=========

1. ABOUT/WHAT
2. TECHNICAL/HOW
3. REQUIREMENTS
4. QUICK REFERENCE

1. ABOUT/WHAT
=============

Almost since the Drupal FAPI introduced the "#collapsible" property for
fieldsets, this property has been a toothless tiger in practical use.
This is because every time you have just submitted a form and it shows
up again with your submission, it does not look like you, the actual user
or admin, left it behind but like the form designer has set it up initially.

With this simple module you may now really profit of collapsible fieldsets
as they will remain in the same state you left them behind.

2. TECHNICAL/HOW
================

The workflow is really simple. Whenever you click on a collapsible fieldset's
title and thus change its state, the new state is being stored individually
and will be recalled as soon as the form is presented the next time.

This may happen

a) per browser (client side storage using cookies)
b) per user (server side storing using Ajax)

where b) overrides a) once you enable it for a role.

3. REQUIREMENTS
===============

There are no further module dependencies. However and as you might have guessed,
Javascript is necessary for this (as is for collapsible fieldsets).

4. QUICK REFERENCE
==================

* Install the module the usual way.
* Set up role permissions for Ajax storage.
* Done!

5. KNOWN ISSUES
===============

Please see the project page at http://drupal.org/project/collapser
