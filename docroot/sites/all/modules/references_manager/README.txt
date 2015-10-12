References Manager
------------------
Provides an optional page for each references field with a drag 'n drop UI
for sorting items in that field.

Installation
------------
* Copy the module to your sites' sites/all/modules/contrib directory.
* On the site's modules page (admin/modules) enable the module.

Configuration
-------------
On each node reference field's settings page there will be a new setting called
"References Manager view mode" which allows the new References Manager page to
be enabled. By default the References Manager is not enabled, selecting a view
mode will enable the page and assign the specific view mode that is used to
display the referenced nodes.

Tips
-----
* Use CSS in your theme to customize the display - hide unnecessary fields,
  shrink text, even set a fixed width, e.g.:

form#references-manager-manage .references-manager-items .references-manager-item .node-teaser h2 {
  font-size: 12px !important;
}

TODO
----
* Expand to support user_references.
* Expand to support entity_references.
* Expand to support all entity types.

Author
------
Written by Damien McKenna [1], sponsored by Bluespark Labs [2].


1. http://drupal.org/user/108450
2. http://www.bluesparklabs.com/
