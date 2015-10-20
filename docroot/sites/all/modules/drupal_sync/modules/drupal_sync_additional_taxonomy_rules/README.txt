Description
-----------
This module is an extension of the basic drupal_sync module.
It allows you to choose in separate nodes which sites should be synchronized.
For that, a node should contain term_reference field. It helps to choose on which
sites this node synchronization should be made.

Installation
-----------
1)Enable the module.
2)Choose content type and taxonomy vocabulary on module configuration page
admin/config/drupal_sync/drupal_sync_additional_taxonomy_rules . In chosen taxonomy
 vocabulary create terms that will mean synchronized sites.
3)Choose corresponding term for the site on basic module configuration page
drupal_sync admin/config/drupal_sync/settings in "Remote sytes" section in
"Choose term for sync rules" field. So from this moment each site will have own
taxonomy term.
4)Add 'term_reference' field with our taxonomy vocabulary created in item 2 to
the node.

How it works
-----------
Creation, changing and deletion of a node will be synchronized only with the sites
that are selected in term_reference node field. If you add term into term_reference
field while node editing, this node will be added into a site that corresponds to
this term. If you delete a term while node editing, this node will be deleted from
corresponding site.