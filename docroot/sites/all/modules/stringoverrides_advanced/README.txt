String Overrides Advanced
=========================


This is a little module that will allow you to override strings in the UI.

We have two reasons for existing:

1. Core provides a nice way to override a handful of strings. But, String
   overrides module makes it incredibly hard to find the string you actually
   need to override. No help is given to a user of Drupal's UI, essentially,
   the module is aimed at developers.
2. Locale module provides a nice UI for searching for strings to override, but,
   by design it does not allow you to translate English strings. It also comes
   with a lot of extra fluff around the edges, and generally does a lot more
   besides just translating strings.


Essentially then this module's purpose is to combine those good things:

1. Allow English strings to be overridden by users of the Drupal UI.
2. Allow those users to search for strings to override.


So, if you don't _need_ your end users to search for the strings in Drupal's UI,
then look elsewhere!


Limitations
-----------

At the moment this only works for English strings, however, it could probably
work for other languages just fine, but really, you're probably after Locale
if you have multiple languages.

There may be a performance impact when using this, but it should be comparable
to locale module's impact.


Conflicts
---------

This module really won't work very well if you enable the String Overrides
module at the same time.
