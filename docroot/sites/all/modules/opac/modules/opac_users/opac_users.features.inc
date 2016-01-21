<?php

/**
 * @file
 * Integrates Features for opac users.
 */


/**
 * Implements hook_features_export_options().
 */
function opac_users_mapping_features_export_options() {
  module_load_include('inc', 'opac_users', "opac_users.db");
  // Load all opac servers.
  $servers = opac_server_load_multiple(FALSE, array(), TRUE);
  $export = array();
  foreach ($servers as $serv_id => $server) {
    // Here we propose to export all users mapping
    // for a given server that have fields mapped.
    // Not each mapping elements.
    $mapping_count = count(opac_users_get_fields_mapping($serv_id));
    if ($mapping_count) {
      $export[$serv_id] = t("Users mapping for @serv_name (@count field(s) mapped)",
        array('@serv_name' => $server->serv_name, '@count' => $mapping_count));
    }
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_users_mapping_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['opac_users'] = 'opac_users';

  foreach (array_keys($data) as $serv_id) {
    // We need some opac_users function to load mapping.
    module_load_include('inc', 'opac_users', "opac_users.db");

    // Add opac_server dependency.
    $export['opac_server'][$serv_id] = $serv_id;

    foreach (opac_users_get_fields_mapping($serv_id) as $mapping) {
      $key = $mapping['serv_id'] . '::' . $mapping['patron_field'];
      $export['features']['opac_users_mapping'][$key] = $key;
    }
  }
  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function opac_users_mapping_features_export_render($module, $data) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  $code = array();
  $code[] = '$mapping = array();';
  foreach ($data as $todo) {
    list($serv_id, $patron_field) = explode('::', $todo);
    // Build features rendering.
    $code[] = "  \$mapping[] = " . features_var_export(opac_users_get_field_mapping($serv_id, $patron_field)) . ";";
  }
  $code[] = "return \$mapping;";
  $code = implode("\n", $code);
  return array('opac_users_features_default_mapping' => $code);
}

/**
 * Implements hook_features_revert().
 */
function opac_users_mapping_features_revert($module_name) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  $mappings = module_invoke($module_name, 'opac_users_features_default_mapping');
  foreach ($mappings as $mapping) {
    opac_users_field_mapping_mod($mapping);
  }
}

/**
 * Implements hook_features_rebuild().
 */
function opac_users_mapping_features_rebuild($module_name) {
  module_load_include('inc', 'opac_users', "opac_users.db");
  $mappings = module_invoke($module_name, 'opac_users_features_default_mapping');
  foreach ($mappings as $mapping) {
    opac_users_field_mapping_mod($mapping);
  }
}
