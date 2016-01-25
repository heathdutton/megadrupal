<?php

/**
 * @todo Why are tools exported as hap[component] = component, while dependencies are dependencies[] = component?
 * @todo Included tools via the dependency pipeline do not display under "Highly Attractive People", only the specified tool is shown.
 * @todo Status of hap inclusion is not shown on the UI (checkboxes are not checked) but are included in the .info file
 */

/**
 * Implements hook_features_api().
 */
function hap_features_api() {
  return array(
    'hap' => array(
      'name' => 'Tools for Highly Attractive People',
      'feature_source' => TRUE,
      'duplicates' => FEATURES_DUPLICATES_ALLOWED,
    ),
  );
}

/**
 * Implements hook_features_export_options().
 */
function hap_features_export_options() {
  $options = array();
  foreach (hap_info() as $name => $info) {
    // Exclude tools that are always enabled
    if (!$info['always enabled']) {
      $options[$name] = $info['name'];
    }
  }
  return $options;
}


/**
 * Implements hook_features_export().
 */
function hap_features_export($data, &$export, $module_name = '') {
  $info = hap_info();
  $pipe = array();

  $processors = array(
    'modules' => 'dependencies',
    'tools' => 'hap',
  );

  foreach ($data as $component) {
    // Add self to exports
    $export['hap'][$component] = $component;

    // Add dependencies to exports via pipeline
    foreach ($processors as $type => $processor) {
      if (isset($info[$component]['dependencies'][$type]) && is_array($info[$component]['dependencies'][$type])) {
        foreach ($info[$component]['dependencies'][$type] as $sub_component) {
          $pipe[$processor][$sub_component] = $sub_component;
        }
      }
    }
  }

  return $pipe;
}


/**
 * Implements hook_features_revert().
 */
function hap_features_revert($module) {
  hap_features_rebuild($module);
}


/**
 * Implements hook_features_rebuild().
 */
function hap_features_rebuild($module) {
  $status = hap_list();

  // Get feature meta data
  $feature = features_get_features($module);

  // If has tool dependencies...
  if (!empty($feature->info['hap'])) {
    $install = array();
    foreach ($feature->info['hap'] as $tools) {
      // ... and tool is disabled...
      if (!$status[$tool]) {
        // ... add to an install queue
        $install[] = $tools;
      }
    }

    // Enable applicable tools
    if (!empty($install)) {
      hap_turn_on($install);
    }
  }
}


/**
 * Implements hook_features_pipe_dependencies_alter().
 */
function hap_features_pipe_dependencies_alter(&$pipe, $data, $export) {
  $hap_list = hap_list();
  foreach ($data as $dependency) {
    $file = drupal_get_path('module', $dependency) . '/' . $dependency . '.info';
    $info = drupal_parse_info_file($file);
    if (isset($info['hap']) && is_array($info['hap'])) {
      foreach ($info['hap'] as $tool) {
        if (isset($hap_list[$tool])) {
          $pipe['hap'][$tool] = $tool;
        }
      }
    }
  }
}
