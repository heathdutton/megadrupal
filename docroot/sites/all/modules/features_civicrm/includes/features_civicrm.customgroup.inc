<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Remove fields which should not be exported.
 */
function customgroup_remove_fields(array $customgroup) {
  unset($customgroup['id']);
  unset($customgroup['extends_entity_column_id']);
  unset($customgroup['extends_entity_column_value']);
  unset($customgroup['created_id']);
  return $customgroup;
}

/**
 * Implements hook_features_export_options().
 */
function customgroup_features_export_options() {
  $options = array();
  $api_customgroup_api = features_civicrm_api('CustomGroup', 'get');
  foreach ($api_customgroup_api['values'] as $customgroup) {
    $options[$customgroup['name']] = $customgroup['name'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function customgroup_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  foreach ($data as $customgroup) {
    $export['features']['customgroup'][$customgroup] = $customgroup;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function customgroup_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_customgroup = array();';
  $code[] = '';
  foreach ($data as $customgroup_key => $customgroup_value) {
    $customgroup_api_result
      = features_civicrm_api('CustomGroup', 'get', array('name' => $customgroup_value));
    $customgroup_id = $customgroup_api_result['id'];
    $customgroup_api_result = $customgroup_api_result['values'][$customgroup_id];
    $customgroup_api_result = customgroup_remove_fields($customgroup_api_result);
    $customgroup_code = features_var_export($customgroup_api_result);
    $code[] = "  \$civicrm_customgroup['{$customgroup_value}'] = {$customgroup_code};";
  }
  $code[] = '  return $civicrm_customgroup;';
  $code = implode("\n", $code);
  return array('civicrm_default_customgroup' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function customgroup_features_rebuild($module) {
  customgroup_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function customgroup_features_revert($module) {
  $civicrm_customgroups = module_invoke($module, 'civicrm_default_customgroup');
  foreach ($civicrm_customgroups as $customgroup) {
    $existing_customgroup = features_civicrm_api('CustomGroup', 'get', array('id' => $customgroup['name']));
    // Merge with code_customgroup.
    $merged_customgroup = array_merge_recursive_distinct($existing_customgroup['values'][$existing_customgroup['id']], $customgroup);
    $api_customgroups_api = features_civicrm_api('CustomGroup', 'create', $merged_customgroup);
  }
}
