Soft Length Limit

This module provides a counter that indicates the max. limit of
characters in a certain text field. The soft limit means that the user
will be warned if the content length of the field is exceeded, but he
will still be able to save all data in the field.


It basically does two things:

* Lets the user configure any instance of a text field's widget, to
  have a specific soft limit. This is done in the usual field settings
  for entity types. This counter will always be shown on fields with a
  soft limit, no matter the settings mentioned below.

* Adds a character countdown to form elements that already have a
  maxlength specified.  This functionality can be customized through
  the options specified below.



-- Configuration of behavior of elements with "maxlength" attribute --

The following Drupal variables can be set to change the criteria for
which elements should have the limit counter:



* (bool) soft_length_limit_maxlength_counter_disabled:

If set to TRUE, no fields with maxlength attribute will have a counter.


* (bool) soft_length_limit_maxlength_counter_admin_only:

If set to TRUE, the counter will only be shown when the admin_theme is
active.


* (Array) soft_length_limit_maxlength_counter_themes:

This as an array of theme names for which the counter should be
shown. If empty, all themes show the counter.


* (Array) soft_limit_length_maxlength_exclude_selectors:

This is an array of jQuery selectors (using classes, IDs etc.) for
elements which should be excluded from having a counter, although they
have a defined maxlength, i.e. date fields, autocomplete fields
etc. could be added here.

* The icons for this came from Creative Commons Attribution 3.0
Unported License, and they came from http://icomoon.io/#icons free
package.
