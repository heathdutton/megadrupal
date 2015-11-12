<?php
/**
 * @file
 * Implements the Features API for export bundles as features.
 */

/**
 * Implements hook_features_export_options().
 */
function fpp_bundles_features_export_options() {
  $options = array();

  /* @var \FppBundle $bundle */
  foreach (fpp_bundles_db()->getAllBundles() as $machine_name => $bundle) {
    $options[$machine_name] = $bundle->getHumanName();
  }

  return $options;
}

/**
 * Implements hook_features_export().
 */
function fpp_bundles_features_export($data, &$export, $feature_name, $module_name) {
  $export['dependencies'][$module_name] = $module_name;
  $prop = 'field' . (FEATURES_API >= 2 ? '_instance' : '');
  $pipe = array();

  foreach ($data as $bundle_machine_name) {
    $export['features'][$module_name][$bundle_machine_name] = $bundle_machine_name;

    $bundle_fields = db_select('field_config_instance', 'fci')
      ->fields('fci', array('field_name'))
      ->condition('bundle', $bundle_machine_name, '=')
      ->execute()
      ->fetchCol();

    foreach ($bundle_fields as $field_name) {
      $field_instance_args = array(FPP_BUNDLES_ENTITY_TYPE, $bundle_machine_name, $field_name);
      $field_instance_id = implode('-', $field_instance_args);
      $field_instance = call_user_func_array('field_info_instance', $field_instance_args);

      if ($field_instance) {
        $export['dependencies'][$field_instance['widget']['module']] = $field_instance['widget']['module'];
      }

      $export['features'][$prop][$field_instance_id] = $field_instance_id;
      $pipe['field_base'][$field_name] = $field_name;
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function fpp_bundles_features_export_render($feature_name, $data, $export = NULL) {
  $code = array();

  foreach ($data as $bundle_machine_name) {
    $bundle = fpp_bundles_db()->getBundleByMachineName($bundle_machine_name);

    if ($bundle) {
      $code += $bundle->export();
    }
  }

  return array(
    FPP_BUNDLES_MACHINE_NAME => '  return ' . features_var_export($code, '  ') . ';',
  );
}

/**
 * Implements hook_features_rebuild().
 */
function fpp_bundles_features_rebuild($feature_name) {
  $db = fpp_bundles_db();

  foreach (module_invoke($feature_name, FPP_BUNDLES_MACHINE_NAME) as $machine_name => $values) {
    if (!$db->getBundleByMachineName($machine_name)) {
      $db->insertBundle($values);
    }
  }

  _fpp_bundles_clear_caches();
}

/**
 * Implements hook_features_revert().
 */
function fpp_bundles_features_revert($feature_name) {
  fpp_bundles_features_rebuild($feature_name);
}
