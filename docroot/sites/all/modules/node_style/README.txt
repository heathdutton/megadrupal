Overview
--------
The node style module allows for per-node customisation of node layouts and
other styling.

Upgrade
-------
If you are upgrading from an older version of this module, ensure that you
visit update.php to update existing database tables. Due to internal Drupal
changes from version to version, some elements which are customised in earlier
versions might not port well to later ones.

Notes
-----
The browser UI provides rudimentary support for styling various elements of a
node page. Advanced tweaks can be accomplished by making use of the two "hooks"
provided by the module for each node style. The hooks are of the form:

hook_node_style_preprocess_SCHEME_MACHINE_NAME()
hook_node_style_process_SCHEME_MACHINE_NAME()

where SCHEME_MACHINE_NAME is the machine name assigned to each node style
scheme. This value can be viewed on each scheme's "edit" page.

The two hooks are extensions of the Drupal PHPTemplate hooks namely:

hook_preprocess() [http://api.drupal.org/api/drupal/modules!system!theme.api.php/function/hook_preprocess/7]
hook_process() [http://api.drupal.org/api/drupal/modules!system!theme.api.php/function/hook_process/7]

The Node Style hooks are structured identically.

Node Style comes with an example style scheme named "Garland Example" (with
machine name "garland_example"), and an example hook implementation named
node_style_node_style_process_garland_example() which can be found in the
file, node_style.module.

Links
-----
* Node style management: admin/structure/node-style
* Node style configuration: admin/structure/node-style/settings
* Project URL: http://drupal.org/project/node_style

Authors
-------
Karthik Kumar/ Zen / |gatsby| : http://drupal.org/user/21209
