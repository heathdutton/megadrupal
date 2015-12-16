<?php

/**
 * Implements hook_features_export().
 */
function gd_infinite_scroll_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['features'] = 'features';
  $export['dependencies']['gd_infinite_scroll'] = 'gd_infinite_scroll';

  foreach ($data as $name) {
    $export['features']['gd_infinite_scroll'][$name] = $name;
  }
  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function gd_infinite_scroll_features_export_options() {
  $feature_types = array();
  $info = gd_infinite_scroll_settings();
  if (!empty($info)) {
    foreach ($info as $name => $settings) {
      $feature_types[$name] = $name;
    }
  }
  return $feature_types;
}

/**
 * Implements hook_features_export_render().
 */
function gd_infinite_scroll_features_export_render($module, $data, $export = NULL) {
  $info = gd_infinite_scroll_settings();

  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $name) {
    if (isset($info[$name]) && $settings = $info[$name]) {
      $output[] = "    '{$name}' => " . features_var_export($settings, '    ') . ",";
    }

  }
  $output[] = '  );';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('gd_infinite_scroll_settings_info' => $output);
}

/**
 * Implements hook_features_revert().
 */
function gd_infinite_scroll_features_revert($module = NULL) {
  // Get default gd infinite scroll settings.
  if (module_hook($module, 'gd_infinite_scroll_settings_info')) {
    $defaults_settings = module_invoke($module, 'gd_infinite_scroll_settings_info');
    $settings = gd_infinite_scroll_settings();
    foreach ($defaults_settings as $name => $default_settings) {
      if (isset($settings[$name])) {
        gd_infinite_scroll_settings_delete($name);
      }
    }
  }
  else {
    drupal_set_message(t('Could not load default settings.'), 'error');
    return FALSE;
  }

  return TRUE;
}

/**
 * Implements hook_features_rebuild().
 */
function gd_infinite_scroll_features_rebuild($module) {
  return gd_infinite_scroll_features_revert($module);
}
