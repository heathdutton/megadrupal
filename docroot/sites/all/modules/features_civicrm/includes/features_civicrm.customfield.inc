<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Remove fields which should not be exported.
 */
function customfield_remove_fields(array $customfield) {
  unset($customfield['id']);
  unset($customfield['custom_group_id']);
  unset($customfield['option_group_id']);
  return $customfield;
}

/**
 * Implements hook_features_export_options().
 */
function customfield_features_export_options() {
  $options = array();
  $api_customfield_api = features_civicrm_api('CustomField', 'get');
  foreach ($api_customfield_api['values'] as $customfield) {
    $options[$customfield['name']] = $customfield['name'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function customfield_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  foreach ($data as $customfield) {
    $api_customfield_api = features_civicrm_api('CustomField', 'get', array('name' => $customfield));
    if ($api_customfield_api['count'] == 1) {
      $custom_group_id = $api_customfield_api['values'][$api_customfield_api['id']]['custom_group_id'];
      $api_customgroup_api = features_civicrm_api('CustomGroup', 'get', array('id' => $custom_group_id));
      if ($api_customgroup_api['count'] == 1) {
        $custom_group_name = $api_customgroup_api['values'][$api_customgroup_api['id']]['name'];
        $export['features']['customgroup'][$custom_group_name] = $custom_group_name;
      }
    }
    $export['features']['customfield'][$customfield] = $customfield;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function customfield_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_customfield = array();';
  $code[] = '';
  foreach ($data as $customfield_key => $customfield_value) {
    $customfield_api_result
      = features_civicrm_api('CustomField', 'get', array('name' => $customfield_value));
    $customfield_id = $customfield_api_result['id'];
    $customfield_api_result = $customfield_api_result['values'][$customfield_id];
    $customgroup_api_result
      = features_civicrm_api('CustomGroup', 'get', array('id' => $customfield_api_result['custom_group_id']));
    $customfield_api_result['custom_group_name'] = $customgroup_api_result['values'][$customfield_api_result['custom_group_id']]['name'];
    $customfield_api_result = customfield_remove_fields($customfield_api_result);
    $customfield_code = features_var_export($customfield_api_result);
    $code[] = "  \$civicrm_customfield['{$customfield_value}'] = {$customfield_code};";
  }
  $code[] = '  return $civicrm_customfield;';
  $code = implode("\n", $code);
  return array('civicrm_default_customfield' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function customfield_features_rebuild($module) {
  customfield_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function customfield_features_revert($module) {
  $civicrm_customfields = module_invoke($module, 'civicrm_default_customfield');
  foreach ($civicrm_customfields as $customfield) {
    $customgroup_id = features_civicrm_api('CustomGroup', 'get', array('name' => $customfield['custom_group_name']));
    $customfield['custom_group_id'] = $customgroup_id['id'];
    unset($customfield['custom_group_name']);
    $existing_customfield = features_civicrm_api('CustomField', 'get', array('name' => $customfield['name']));
    // Merge with code_customfield.
    $merged_customfield = array_merge_recursive_distinct($existing_customfield['values'][$existing_customfield['id']], $customfield);
    $api_customfields_api = features_civicrm_api('CustomField', 'create', $merged_customfield);
  }
}
