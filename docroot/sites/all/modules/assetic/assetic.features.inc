<?php

/**
 * Implements hook_features_export().
 */
function assetic_features_export($data, &$export, $module_name = '') {
  // The assetic_default_filters() hook integration is provided by the
  // features module so we need to add it as a dependency.
  $export['dependencies']['features'] = 'features';

  $filters = assetic_get_filter_list();
  foreach ($data as $name) {
    if (isset($filters[$name])) {
      $export['features']['assetic'][$name] = $name;
      $export['dependencies'][$filters[$name]['module']] = $filters[$name]['module'];
    }
  }

  $pipe = array();
  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function assetic_features_export_options() {
  $options = array();
  foreach (assetic_get_filter_info() as $filter_name => $filter_info) {
    $options[$filter_name] = $filter_info['title'];
  }

  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function assetic_features_export_render($module, $data, $export = NULL) {
  $code = array();
  $code[] = '  $filters = array();';
  $code[] = '';

  $filters = assetic_get_filter_list();
  foreach ($data as $name) {
    if (isset($filters[$name])) {
      $filter = $filters[$name];

      // Remove everything that doesn't need to be exported.
      foreach (array('admin', 'apply to', 'class', 'constructor callback', 'debug mode', 'filter', 'title') as $unset_key) {
        unset($filter[$unset_key]);
      }

      $filter_export = features_var_export($filter, '  ');

      $filter_alias = features_var_export($name);
      $code[] = "  // Exported filter: {$filters[$name]['title']}.";
      $code[] = "  \$filters[{$filter_alias}] = {$filter_export};";
      $code[] = "";
    }
  }

  $code[] = '  return $filters;';
  $code = implode("\n", $code);

  return array('assetic_features_default_filters' => $code);
}

/**
 * Implements hook_features_revert().
 */
function assetic_features_revert($module) {
  return assetic_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function assetic_features_rebuild($module) {
  if ($defaults = features_get_default('assetic', $module)) {
    foreach ($defaults as $filter) {
      assetic_filter_save($filter);
    }
  }
}
