<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function taxonomy_dqx_adminmenu_items($api) {

  // TODO: Why are these not shown by default?
  $api->forceShowItem('admin/structure/taxonomy/list');
  $api->forceShowItem('admin/structure/taxonomy/add');
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function taxonomy_dqx_adminmenu_items_alter($api) {

  $base = 'admin/structure/taxonomy';

  foreach (taxonomy_get_vocabularies() as $vocabulary) {
    $slug = $vocabulary->machine_name;
    $api->moveItem("$base/$slug", "$base/list/$slug");
    $api->parentMap("$base/$slug", "$base/list/$slug");
    $api->submenuSetInlineChild("$base/list/$slug", "$base/$slug/fields");
  }

  $api->submenuSetInlineChild($base, "$base/list");
}
