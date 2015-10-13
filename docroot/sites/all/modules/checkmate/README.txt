README.txt
==========

-- USAGE --

* Install the Checkmate and Checkmate Reference module.
* Create a new checkmate item type (Admin -> Structure -> Checkmate item types).
* Create some checks of this new checkmate item type 
(Content -> Checkmate checks -> Add checkmate item).
* Create an entity type (e.g. a content type) 
and add a checkmate reference field.
* Create an entity of that entity type and reference the checks.
* Go to the entity display and there you'll find your checklist.
* Checkmate has different ways of updating checks. 
There are the default manual check, where a user is responsible 
for setting the status, and the more complex additional checks
that will be automatically updated using callback functions.

-- EXTRAS --

* Install the Checkmate Sample Module for a predefined set of checks. 
This will create a checkmate item type, a content type containing a
checkmate reference field and some checkmate item entities.

* Checkmate is built on the Entity API, which means that checkmate 
checks and checkmate item statuses are extendable. Browse to the 
checkmate item type configuration page for adding extra fields
(Admin -> Structure -> Checkmate item types).

* Checkmate plays nice with Rules the Checkmate Sample Module 
adds a preconfigured rule to send e-mails each time a checkmate
item is updated. You can change the e-mails under the Rules 
configuration page (Admin -> Configuration -> Workflow -> 
Rules -> Edit "Send mail after checkmate status creation").

-- FOR DEVELOPERS --

Checkmate gives developers the ability to add their own status callbacks.
See checkmate.api.php for an example.

-- CONTACT --

Current maintainers:
* Core functionality: Geert van Dort (geertvd) - http://drupal.org/user/536694
* Theming and usability: Jens Braet (JensBraet) - http://drupal.org/user/963394
* Feature requests: Pascal Noppe (tafkas) - http://drupal.org/user/254362

This project has been sponsored by:
XIO - www.xio.be
