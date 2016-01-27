<?php

/**
 * @file
 * Integrates Features for opac items.
 */


/**
 * Implements hook_features_export_options().
 */
function opac_items_mapping_features_export_options() {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  // Load all opac servers.
  $servers = opac_server_load_multiple(FALSE, array(), TRUE);
  $export = array();
  foreach ($servers as $serv_id => $server) {
    // Here we propose to export all items mapping
    // for a given server that have fields mapped.
    // Not each mapping elements.
    $mapping_count = count(opac_items_get_fields_mapping($serv_id));
    if ($mapping_count) {
      $export[$serv_id] = t('Items mapping for @serv_name (@count field(s) mapped)',
        array('@serv_name' => $server->serv_name, '@count' => $mapping_count));
    }
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_items_mapping_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['opac_items'] = 'opac_items';

  foreach (array_keys($data) as $serv_id) {
    // We need some opac_items function to load mapping.
    module_load_include('inc', 'opac_items', "includes/opac_items.db");

    // Add opac_server dependency.
    $export['opac_server'][$serv_id] = $serv_id;

    // Export items mapping.
    foreach (opac_items_get_fields_mapping($serv_id) as $mapping) {
      $key = $mapping['serv_id'] . '::' . $mapping['item_field'] . '::' . $mapping['display_header'];
      $export['features']['opac_items_mapping'][$key] = $key;
    }
  }
  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function opac_items_mapping_features_export_render($module, $data) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $code = array();
  $code[] = '$mapping = array();';
  foreach ($data as $todo) {
    list($serv_id, $item_field, $header) = explode('::', $todo);
    // Build features rendering.
    $code[] = "  \$mapping[] = " . features_var_export(opac_items_get_field_mapping($serv_id, $item_field)) . ";";
  }
  $code[] = "return \$mapping;";
  $code = implode("\n", $code);
  return array('opac_items_features_default_mapping' => $code);
}

/**
 * Implements hook_features_revert().
 */
function opac_items_mapping_features_revert($module_name) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $mappings = module_invoke($module_name, 'opac_items_features_default_mapping');
  foreach ($mappings as $mapping) {
    opac_items_field_mapping_del($mapping['serv_id'], $mapping['item_field']);
  }
}

/**
 * Implements hook_features_rebuild().
 */
function opac_items_mapping_features_rebuild($module_name) {
  module_load_include('inc', 'opac_items', "includes/opac_items.db");
  $mappings = module_invoke($module_name, 'opac_items_features_default_mapping');
  foreach ($mappings as $mapping) {
    opac_items_field_mapping_add($mapping);
  }
}
