<?php
/**
 * @file
 * Features file for the workbench_scheduler module.
 */

/**
 * Implements hook_features_api().
 */
function workbench_scheduler_features_api() {
  return array(
    'workbench_scheduler' => array(
      'name' => t('Workbench Schedules'),
      'default_hook' => 'workbench_scheduler_export_schedules',
      'feature_source' => TRUE,
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'file' => drupal_get_path('module', 'workbench_scheduler') . '/workbench_scheduler.features.inc',
    ),
  );
}

/**
 * Implements COMPONENT_features_export_options().
 *
 * Inform features about the available schedules in the database.
 */
function workbench_scheduler_features_export_options() {
  return db_select('workbench_scheduler_schedules', 'wss')
    ->fields('wss', array('name', 'name'))
    ->execute()
    ->fetchAllKeyed();
}

/**
 * Implements COMPONENT_features_export().
 *
 * Process the features export array for schedules.
 */
function workbench_scheduler_features_export($data, &$export, $module_name) {
  $export['dependencies']['workbench_scheduler'] = 'workbench_scheduler';

  foreach ($data as $component) {
    $export['features']['workbench_scheduler'][$component] = $component;
  }

  return array();
}


/**
 * Implements COMPONENT_features_export_render().
 *
 * Render workbench schedules as code.
 */
function workbench_scheduler_features_export_render($module_name, $data) {
  $items = array();
  foreach ($data as $schedule) {
    $items[$schedule] = workbench_scheduler_load_schedules($schedule);

    // Unset SID to prevent overridden state.
    unset($items[$schedule]->sid);
  }

  $code = "  \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';

  return array('workbench_scheduler_export_schedules' => $code);
}


/**
 * Implements COMPONENT_features_revert().
 */
function workbench_scheduler_features_revert($module) {
  workbench_scheduler_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_enable_feature().
 */
function workbench_scheduler_features_enable_feature($module) {
  workbench_scheduler_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_rebuild().
 *
 * Store each exported schedule in the database.
 */
function workbench_scheduler_features_rebuild($module) {
  if ($defaults = features_get_default('workbench_scheduler', $module)) {
    foreach ($defaults as $schedule) {
      workbench_scheduler_save_schedule($schedule['name'], $schedule);
    }
  }
  drupal_static_reset('workbench_scheduler_load_schedules');
}
