Views Default User Taxonomy
===========================

This module allows you to create lists of users based on shared taxonomy
terms in user profile fields.

Out of the box, Views does this only for nodes. This module overrides the
plugin that manages this, and adds user support.


INSTALLATION
------------

Extract the module to sites/all/modules and enable it via the modules
administration page. You will find it under the 'Views' category.


USAGE
-----

* Create a new User view with a block display
* Add a relationship for a term reference field.
* Add a contextual filter for 'Taxonomy term: Term ID' and configure it to
  use the relationship you just created.
* Select 'Provide default value' and choose 'Taxonomy term ID from URL' from
  the drop-down box.
* Check the box for 'Load default filter from user page, that's good for
  related taxonomy blocks' and optionally limit the vocabulary.
* Optionally, add a second contextual filter to exclude the current user
  from the views results.
* Stick the new block in a region and make sure it displays on user/% URLs.


BUGS
----

No bugs, please.
