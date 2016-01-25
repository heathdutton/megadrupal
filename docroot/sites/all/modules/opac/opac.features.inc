<?php

/**
 * @file
 * Integrates Features for opac.
 */

/**
 * Implements hook_features_export_options().
 */
function opac_server_features_export_options() {
  $servers = opac_server_load_multiple(FALSE, array('serv_enabled' => 1));
  $export = array();
  foreach ($servers as $server) {
    $export[$server->serv_id] = $server->serv_name;
  }
  return $export;
}

/**
 * Implements hook_features_export_options().
 */
function opac_content_type_features_export_options() {
  module_load_include('inc', 'opac', "includes/opac.db");
  $types = opac_get_content_types();
  // Load all opac servers.
  $servers = opac_server_load_multiple(FALSE, array(), TRUE);
  $export = array();
  foreach ($servers as $serv_id => $server) {
    // Get content types related to this server.
    $opac_types = opac_get_content_types($serv_id);
    foreach ($opac_types as $type_name => $type) {
      $string = t('Mapping for @serv_name/@type', array('@serv_name' => $server->serv_name, '@type' => $type['content_type']));
      $export["$serv_id::$type_name"] = $string;
    }
  }
  return $export;
}

/**
 * Implements hook_features_export_options().
 */
function opac_mapping_features_export_options() {
  module_load_include('inc', 'opac', "includes/opac.db");
  // Load all opac servers.
  $servers = opac_server_load_multiple(FALSE, array(), TRUE);
  $export = array();
  foreach ($servers as $serv_id => $server) {
    // Get content types related to this server.
    $opac_types = opac_get_content_types($serv_id);
    foreach ($opac_types as $type_name => $type) {
      $mapping_count = count(opac_get_server_mapping($serv_id, $type_name));
      if ($mapping_count) {
        $string = 'Mapping for @serv_name/@type (@count field(s) mapped)';
        $t = array(
          '@serv_name' => $server->serv_name,
          '@type' => $type_name,
          '@count' => $mapping_count,
        );
        $export["$serv_id::$type_name"] = t($string, $t);
      }
    }
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_content_type_features_export($data, &$export, $module_name = '') {
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // OPAC dependencies.
    $export['opac_server'][$serv_id] = $serv_id;
    $export['node'][$type_name] = $type_name;

    // OPAC Content types.
    $export['features']['opac_content_type']["$serv_id::$type_name"] = "$serv_id::$type_name";
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_mapping_features_export($data, &$export, $module_name = '') {
  module_load_include('inc', 'opac', "includes/opac.db");
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // Content type dependencies.
    $export['opac_content_type']["$serv_id::$type_name"] = "$serv_id::$type_name";

    // Mappings.
    $mappings = opac_get_server_mapping($serv_id, $type_name);
    foreach (array_keys($mappings) as $node_field) {
      $export['features']['opac_mapping']["$serv_id::$type_name::$node_field"] = "$serv_id::$type_name::$node_field";
    }
  }
  return $export;
}

/**
 * Implements hook_features_export().
 */
function opac_server_features_export($data, &$export, $module_name = '') {
  module_load_include('inc', 'opac', "includes/opac.db");
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['opac'] = 'opac';

  $servers = opac_server_load_multiple(FALSE, array('serv_enabled' => 1));
  foreach ($servers as $id => $server) {
    if (in_array($id, $data)) {
      $export['features']['opac_server'][$id] = $id;
    }
  }
  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function opac_content_type_features_export_render($module, $data) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $code = array();
  $code[] = '$opac_types = array();';
  foreach ($data as $todo) {
    list($serv_id, $type_name) = explode('::', $todo);
    // Build features rendering.
    $code[] = "  \$opac_types[] = " . features_var_export(opac_get_content_type($serv_id, $type_name)) . ";";
  }
  $code[] = "return \$opac_types;";
  $code = implode("\n", $code);
  return array('opac_features_default_opac_content_type' => $code);
}

/**
 * Implements hook_features_export_render().
 */
function opac_mapping_features_export_render($module, $data) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $code = array();
  $code[] = '$opac_mappings = array();';
  foreach ($data as $todo) {
    list($serv_id, $type_name, $node_field) = explode('::', $todo);
    // Build features rendering.
    $code[] = "  \$opac_mappings[] = " . features_var_export(opac_get_mapping($serv_id, $type_name, $node_field)) . ";";
  }
  $code[] = "return \$opac_mappings;";
  $code = implode("\n", $code);
  return array('opac_features_default_opac_mapping' => $code);
}

/**
 * Implements hook_features_export_render().
 */
function opac_server_features_export_render($module, $data) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $code = array();
  $code[] = '$servers = array();';
  $servers = opac_server_load_multiple(FALSE, array('serv_enabled' => 1));
  foreach ($data as $name) {
    if (in_array($name, array_keys($servers))) {
      // Build features rendering.
      $code[] = "  \$servers['{$name}'] = " . features_var_export(opac_object2array($servers[$name])) . ";";
    }
  }
  $code[] = "return \$servers;";
  $code = implode("\n", $code);
  return array('opac_features_default_opac_server' => $code);
}

/**
 * Implements hook_features_revert().
 */
function opac_content_type_features_revert($module_name) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $opac_types = module_invoke($module_name, 'opac_features_default_opac_content_type');
  foreach ($opac_types as $serv_id => $types) {
    foreach ($types as $type_name => $type) {
      opac_content_type_delete($type);
    }
  }
}

/**
 * Implements hook_features_revert().
 */
function opac_server_features_revert($module_name) {
  $servers = module_invoke($module_name, 'opac_features_default_opac_server');
  if (!empty($servers)) {
    // Delete servers.
    entity_delete_multiple('opac_server', array_keys($servers));
  }
}

/**
 * Implements hook_features_revert().
 */
function opac_mapping_features_revert($module_name) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $mappings = module_invoke($module_name, 'opac_features_default_opac_mapping');
  foreach ($mappings as $mapping) {
    opac_mapping_del($mapping);
  }
}

/**
 * Implements hook_features_rebuild().
 */
function opac_content_type_features_rebuild($module) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $opac_types = module_invoke($module, 'opac_features_default_opac_content_type');
  foreach ($opac_types as $type) {
    opac_content_type_add($type);
  }
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds opac servers from code defaults.
 */
function opac_server_features_rebuild($module) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $saved_servers = module_invoke($module, 'opac_features_default_opac_server');
  foreach ($saved_servers as $key => $server) {
    // Create opac server.
    $new = entity_create('opac_server', $server);
    $new->save();
  }
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds opac servers from code defaults.
 */
function opac_mapping_features_rebuild($module) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $mappings = module_invoke($module, 'opac_features_default_opac_mapping');
  foreach ($mappings as $mapping) {
    opac_mapping_add($mapping);
  }
}

/**
 * Transform PHP object to array.
 *
 * @param object $object
 *   A PHP object.
 *
 * @return array
 *   An array.
 */
function opac_object2array($object) {
  $array = array();
  if (is_object($object)) {
    foreach ($object as $key => $value) {
      $array[$key] = $value;
    }
  }
  else {
    $array = $object;
  }
  return $array;
}
