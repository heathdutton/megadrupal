<?php

/**
 * @file
 * Integrates Features for opac_authorities.
 */

/**
 * Implements hook_features_export_options().
 */
function opac_authorities_mapping_features_export_options() {
  module_load_include('inc', 'opac', 'includes/opac.db');
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  // Load all opac servers.
  $servers = opac_server_load_multiple(FALSE, array(), TRUE);
  $export = array();
  foreach ($servers as $serv_id => $server) {
    // Get content types related to this server.
    $opac_types = opac_authorities_get_server_types($serv_id);
    foreach ($opac_types as $type_name => $type) {
      $mappings = opac_authorities_get_mappings(array(
        'servers' => $serv_id,
        'authority_types' => $type_name,
      ));
      $mapping_count = count($mappings);
      if ($mapping_count) {
        $t_options = array(
          '@serv_name' => $server->serv_name,
          '@type' => $type_name,
          '@count' => $mapping_count,
        );
        $string = t('Authority mapping for @serv_name/@type (@count field(s) mapped)', $t_options);
        $export["$serv_id::$type_name"] = $string;
      }
    }
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_authorities_mapping_features_export($data, &$export, $module_name = '') {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $pipe = array();
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // Authority mapping dependencies.
    $pipe['opac_authority_type'][$type_name] = $type_name;
    $pipe['opac_server'][$serv_id] = $serv_id;
    $pipe['opac_authorities_server_type']["$serv_id::$type_name"] = "$serv_id::$type_name";

    // Mappings.
    $export['features']['opac_authorities_mapping']["$serv_id::$type_name"] = "$serv_id::$type_name";
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function opac_authorities_mapping_features_export_render($module, $data) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $code = array();
  $code[] = '  $mappings = array();';
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // Build features rendering.
    $mappings = opac_authorities_get_mappings(array(
      'servers' => $serv_id,
      'authority_types' => $type_name,
    ));
    foreach ($mappings as $mapping) {
      $code[] = '  $mappings[] = ' . features_var_export($mapping, '  ') . ';';
    }
  }
  $code[] = '  return $mappings;';
  $code = implode("\n", $code);
  return array('opac_authorities_features_default_mapping' => $code);
}

/**
 * Implements hook_features_revert().
 */
function opac_authorities_mapping_features_revert($module) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $defaults = module_invoke($module, 'opac_authorities_features_default_mapping');
  // Delete mappings of servers and authtype added by this feature.
  $delete = array();
  foreach ($defaults as $default) {
    $serv_id = $default['serv_id'];
    $authtype = $default['authtype'];
    $delete["$serv_id::$authtype"] = 1;
  }
  foreach (array_keys($delete) as $d) {
    list($serv_id, $authtype) = explode('::', $d);
    $mappings = opac_authorities_get_mappings(array(
      'servers' => $serv_id,
      'authority_types' => $authtype,
    ));
    foreach ($mappings as $mapping) {
      opac_authorities_mapping_del($mapping);
    }
  }
  // Add defaults.
  foreach ($defaults as $default) {
    opac_authorities_mapping_add($default);
  }
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds opac servers from code defaults.
 */
function opac_authorities_mapping_features_rebuild($module) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $mappings = module_invoke($module, 'opac_authorities_features_default_mapping');
  foreach ($mappings as $mapping) {
    opac_authorities_mapping_add($mapping);
  }
}

/**
 * Implements hook_features_export_options().
 */
function opac_authorities_server_type_features_export_options() {
  module_load_include('inc', 'opac', 'includes/opac.db');
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  // Load all opac servers.
  $servers = opac_server_load_multiple(FALSE, array(), TRUE);
  $export = array();
  foreach ($servers as $serv_id => $server) {
    // Get content types related to this server.
    $opac_types = opac_authorities_get_server_types($serv_id);
    foreach ($opac_types as $type_name => $type) {
      $string = "Authority type '@type' for server '@serv_name'";
      $variables = array(
        '@serv_name' => $server->serv_name,
        '@type' => $type_name,
      );
      $t_options = array('@serv_name' => $server->serv_name, '@type' => $type_name);
      $string = t("Authority type '@type' for server '@serv_name'", $t_options);
      $export["$serv_id::$type_name"] = $string;
    }
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_authorities_server_type_features_export($data, &$export, $module_name = '') {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $pipe = array();
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // Authority type dependencies.
    $pipe['opac_authority_type'][$type_name] = $type_name;
    $pipe['opac_server'][$serv_id] = $serv_id;

    $export['features']['opac_authorities_server_type']["$serv_id::$type_name"] = "$serv_id::$type_name";
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function opac_authorities_server_type_features_export_render($module, $data) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $code = array();
  $code[] = '  $server_types = array();';
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // Build features rendering.
    $server_types = opac_authorities_get_server_types($serv_id, $type_name);
    foreach ($server_types as $server_type) {
      $code[] = '  $server_types[] = ' . features_var_export($server_type, '  ') . ';';
    }
  }
  $code[] = '  return $server_types;';
  $code = implode("\n", $code);
  return array('opac_authorities_features_default_server_type' => $code);
}

/**
 * Implements hook_features_revert().
 */
function opac_authorities_server_type_features_revert($module_name) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $server_types = module_invoke($module_name, 'opac_authorities_features_default_server_type');
  foreach ($server_types as $server_type) {
    opac_authorities_server_type_edit($server_type);
  }
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds opac servers from code defaults.
 */
function opac_authorities_server_type_features_rebuild($module) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  $server_types = module_invoke($module, 'opac_authorities_features_default_server_type');
  foreach ($server_types as $server_type) {
    opac_authorities_server_type_add($server_type);
  }
}
