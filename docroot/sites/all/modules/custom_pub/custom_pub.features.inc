<?php

/**
 * Implements hook_features_export_options().
 */
function custom_pub_features_export_options() {
  return custom_pub_types_list();
}

/**
 * Implements hook_features_export.
 */
function custom_pub_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $map = features_get_default_map('custom_pub');
  $pub_types = array_keys(custom_pub_types_list());

  foreach ($data as $option) {
    // Poll node module to determine who provides the option.
    if (in_array($option, $pub_types)) {
      // If this option is provided by a different module, add it as a dependency
      if (isset($map[$option]) && $map[$option] != $module_name) {
        $export['dependencies'][$map[$option]] = $map[$option];
      }
      // Otherwise export the option
      else {
        $export['features']['custom_pub'][$option] = $option;
        $export['dependencies']['custom_pub'] = 'custom_pub';
      }
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function custom_pub_features_export_render($module, $data, $export = NULL) {
  $pub_types = variable_get('custom_pub_types', array());
  $pub_type_names = array_keys($pub_types);
  $output = array();
  $output[] = '  $options = array();';
  foreach ($data as $option_name) {
    if (in_array($option_name, $pub_type_names)) {
      $option_node_types = $pub_types[$option_name]['node_types'];
      $output[] = "  // Exported option: {$option_name}";
      $output[] = "  \$options['{$option_name}'] = array(";
      $output[] = "    'type' => '{$option_name}',";
      $output[] = "    'name' => t('{$pub_types[$option_name]['name']}'),";
      $output[] = "    'node_types' => array(";
      foreach ($option_node_types as $node_type => $node_name) {
        $output[] = "      '{$node_type}' => t('{$node_name}'),";
      }
      $output[] = "    ),";
      $output[] = "  );";
      $output[] = "";
    }
  }
  $output[] = '  return $options;';
  $output = implode("\n", $output);
  return array('custom_pub_defaults' => $output);
}

/**
 * Implements hook_features_revert().
 *
 * @param $module
 * name of module to revert content for
 */
function custom_pub_features_revert($module = NULL) {
  if ($default_options = features_get_default('custom_pub', $module)) {
    $pub_types = variable_get('custom_pub_types', array());
    foreach ($default_options as $option_name => $option_info) {
      if (isset($pub_types[$option_name])) {
        unset($pub_types[$option_name]);
      }
    }
    variable_set('custom_pub_types', $pub_types);
    custom_pub_types_rebuild();
  }
}

/**
 * Implements hook_features_disable_feature().
 *
 * @param $module
 *   Name of module that has been disabled.
 */
function custom_pub_features_disable_feature($module) {
  if ($default_options = features_get_default('custom_pub', $module)) {
    $pub_types = variable_get('custom_pub_types', array());
    foreach ($default_options as $option_name => $option_info) {
      if (isset($pub_types[$option_name])) {
        unset($pub_types[$option_name]['default']);
      }
    }
    variable_set('custom_pub_types', $pub_types);
    // Rebuild schema
    cache_clear_all('schema', 'cache');
  }
}

/**
 * Implements hook_features_enable_feature().
 *
 * @param $module
 *   Name of module that has been enabled.
 */
function custom_pub_features_enable_feature($module) {
  if ($default_options = features_get_default('custom_pub', $module)) {
    $spec = array(
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0
     );
    // Add fields in database
    foreach ($default_options as $option_name => $option_info) {
      if (!db_field_exists('node', $option_name)) {
        $spec['description'] = "Custom publishing option $option_name";
        db_add_field('node', $option_name, $spec);
      }
    }

    custom_pub_types_rebuild();
    // Rebuild schema
    cache_clear_all('schema', 'cache');
  }
}
