
-- SUMMARY --

The Term Sibling Reorder module allows taxonomy term administrators to reorder
a term's siblings from the add or edit page for a term.

This is especially helpful if you're using taxonomy for something more than
categorization. If you have a list of terms in a block for navigation with
Views, or if you're using Taxonomy Menu with the sync option, this module can
let you add a term without temporarily affecting your site's output.

For a full description of the module, visit the project page:
  http://drupal.org/project/term_sibling_reorder

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/term_sibling_reorder


-- FEATURES --

* Place a term within a hierarchy from the add/edit form.
* Reorder siblings to a term from that term's add/edit form.
* Integrates with Taxonomy Menu if you have it configured for the vocabulary (see below).
* Works well with with Taxonomy term Views (if you use the "Taxonomy term: Weight" sort).

-- REQUIREMENTS --

None.


-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

There is no configuration to speak of, just enable it and browse to a term
add/edit page. The reorder widget can be found inside of the Relations
fieldset.

There are no permissions to configure, as it is shown on term add/edit forms.
Users which are able to access those forms will be able to use the reorder form
instead of the weight field.


To integrate with Taxonomy Menu:

* As of May 12th, you must install the 7.x-1.x-dev release (I expect it will make it into a 7.x-1.3 release when there is one).
* Configure Taxonomy Menu for the vocabulary as you need to.
* Ensure the "Synchronise changes to this vocabulary" option is checked.


-- CREDITS --

Sponsored by CASRAI (http://casrai.org), developed by ClikFocus (http://clikfocus.com). This module is a small piece of a larger open source project we’re calling OneWeb and hoping to release later this year. OneWeb is a part of a planned CASRAI initiative called Research Commons.
