MENU TO TAXONOMY
================

This module is similar to the taxonomy_menu module (which synchronizes a
taxonomy to a menu), except it synchronizes a menu to a taxonomy instead.

Any new menu links for a specific menu will have a new, corresponding taxonomy
term if the vocabulary has been configured to synchronize to a menu.

In the settings of a vocabulary you can set up whether to synchronize any menu
to the vocabulary in question. This way, whenever a menu link is added, a
matching taxonomy term will be added as well.


SUBMODULES
==========

The (optional) "Menu to Taxonomy Assign" submodule allows you to have Term
references that will be automatically assigned to any terms synchronized to
menu links pointing to the node containing the term reference.

In order to use this functionality, add a term reference on a node and in the
field settings check the "Allow automatic assignment" checkbox. Make sure that
the relevant vocabularies are allowed in the "Allowed vocabularies" option.

Using the "Menu to Taxonomy Assign" submodule, whenever a node or menu link is
added/updated, the term references in the relevant node will point to any terms
syncronized to menu links belonging to the node in question.