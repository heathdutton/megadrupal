Taxonomy Term Status
====================

This module adds a status-flag to taxonomy terms. Using this flag it is
possible to specify whether terms should be published or not. Users with the
appropriate permission may access unpublished terms.

The module also integrates with views by exporting the term status as a views
field. It also exposes actions which may be used for integration with rules
and views bulk operations.


Installation
------------
1.  Copy the termstatus module folder into sites/all/modules and enable it in
    your drupal installation.
2.  Review and set permissions in Admin -> People -> Permissions
3.  Rebuild term status records for existing terms in
    Admin -> Config -> System -> Taxonomy Term Status
4.  Edit your vocabularies and specify the default status for newly created
    terms. (optional).
