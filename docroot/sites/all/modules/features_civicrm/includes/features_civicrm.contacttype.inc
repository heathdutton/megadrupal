<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Remove fields which should not be exported.
 */
function contacttype_remove_fields(array $contacttype) {
  unset($contacttype['id']);
  return $contacttype;
}

/**
 * Implements hook_features_export_options().
 */
function contacttype_features_export_options() {
  $options = array();
  $api_contacttype_api = features_civicrm_api('ContactType', 'get');
  foreach ($api_contacttype_api['values'] as $contacttype) {
    $options[$contacttype['name']] = $contacttype['name'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function contacttype_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  foreach ($data as $contacttype) {
    $api_contacttype_api = features_civicrm_api('ContactType', 'get', array('name' => $contacttype));
    if (isset($api_contacttype_api['values'][$api_contacttype_api['id']]['parent_id'])) {
      if ($api_contacttype_api['values'][$api_contacttype_api['id']]['parent_id'] == 1) {
        $export['features']['contacttype']['Individual'] = 'Individual';
      }
      elseif ($api_contacttype_api['values'][$api_contacttype_api['id']]['parent_id'] == 2) {
        $export['features']['contacttype']['Household'] = 'Household';
      }
      elseif ($api_contacttype_api['values'][$api_contacttype_api['id']]['parent_id'] == 3) {
        $export['features']['contacttype']['Organization'] = 'Organization';
      }
    }
    $export['features']['contacttype'][$contacttype] = $contacttype;
  }
  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function contacttype_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_contacttype = array();';
  $code[] = '';
  foreach ($data as $contacttype_key => $contacttype_value) {
    $contacttype_api_result
      = features_civicrm_api('ContactType', 'get', array('name' => $contacttype_value));
    if ($contacttype_api_result['count'] == 1) {
      $contacttype_id = $contacttype_api_result['id'];
      $contacttype_api_result = $contacttype_api_result['values'][$contacttype_id];
      $contacttype_api_result = contacttype_remove_fields($contacttype_api_result);
      $contacttype_code = features_var_export($contacttype_api_result);
      $code[] = "  \$civicrm_contacttype['{$contacttype_value}'] = {$contacttype_code};";
    }
  }
  $code[] = '  return $civicrm_contacttype;';
  $code = implode("\n", $code);
  return array('civicrm_default_contacttype' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function contacttype_features_rebuild($module) {
  contacttype_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function contacttype_features_revert($module) {
  $civicrm_contacttypes = module_invoke($module, 'civicrm_default_contacttype');
  foreach ($civicrm_contacttypes as $contacttype) {
    $existing_contacttype = features_civicrm_api('ContactType', 'get', array('name' => $contacttype['name']));
    // Merge with code_contacttype.
    $merged_contacttype = array_merge_recursive_distinct($existing_contacttype['values'][$existing_contacttype['id']], $contacttype);
    $api_contacttypes_api = features_civicrm_api('ContactType', 'create', $merged_contacttype);
  }
}
