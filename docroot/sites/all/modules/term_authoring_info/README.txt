
Description
-----------
The Taxonomy Term Authoring Inormation module store authoring information
(author, published date and updated date) for the taxonomy term.

A configuration option will be added in vocabulary add/edit form to
enable/disable authoring information fields in the taxonomy term add/edit form.

Exposes "authoring information fields" in Field UI's "Manage display" pages let
users re-order and control the visibility.

This module is views integrated. Exposes authoring information fields (author,
published date and updated date) to views configuration
(fields/filters/arguments/sorts/relation).

This module also exposes a hook (hook_term_authoring_info_presave) for
developers so that term authoring information can be alter before inserted or
updated.

This module comes with its own database table to store authoring information
per taxonomy term (tid).

Installation
------------
1) Place this module directory in your "modules" folder (this will
usually be "sites/all/modules/"). Don't install your module in Drupal
core's "modules" folder, since that will cause problems and is bad
practice in general. If "sites/all/modules" doesn't exist yet, just
create it.

2) Enable the module.

3) Go to taxonomy vocabulary add/edit form to enable/disable authoring
information fields in the taxonomy term add/edit form.

Dependencies
------------
1. Taxonomy
2. Views

Author
------
Pijus Kanti Roy
pijus.k.roy@gmail.com
