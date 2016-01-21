<?php

/**
 * Implements hook_features_export().
 */
function hansel_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['hansel'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 */
function hansel_features_export_options() {
  return array(
    'config' => t('Configuration'),
    'settings' => t('Settings'),
  );
}

/**
 * Implements hook_features_export_render().
 */
function hansel_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'hansel', 'hansel.export');
  $code = '';
  $code = "  \$elements = array();\n";
  $object = array();
  foreach ($data as $element) {
    switch ($element) {
      case 'config':
        $object = hansel_export_config();
        break;
      case 'settings':
        $object = hansel_export_settings();
        break;
    }
    $code .= "  \$elements['{$element}'] = " . features_var_export($object, '  ') . ";\n";
  }
  $code .= "  return \$elements;";
  return array('hansel_default' => $code);
}

/**
 * Implements hook_features_revert().
 */
function hansel_features_revert($module_name) {
  module_load_include('inc', 'hansel', 'hansel.export');

  $config = module_invoke($module_name, 'hansel_default');
  if (!empty($config)) {
    foreach ($config as $element => $object) {
      switch ($element) {
        case 'config':
          hansel_import_config($object);
          break;
        case 'settings':
          hansel_import_settings($object);
          break;
      }
    }
  }
}

/**
 * Implements hook_features_rebuild().
 */
function hansel_features_rebuild($module_name) {
  module_load_include('inc', 'hansel', 'hansel.export');

  $config = module_invoke($module_name, 'hansel_default');
  if (!empty($config)) {
    foreach ($config as $element => $object) {
      switch ($element) {
        case 'config':
          hansel_import_config($object);
          break;
        case 'settings':
          hansel_import_settings($object);
          break;
      }
    }
  }
}
