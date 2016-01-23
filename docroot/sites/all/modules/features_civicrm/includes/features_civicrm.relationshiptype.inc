<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Remove fields which should not be exported.
 */
function relationshiptype_remove_fields(array $relationshiptype) {
  unset($relationshiptype['id']);
  return $relationshiptype;
}

/**
 * Implements hook_features_export_options().
 */
function relationshiptype_features_export_options() {
  $options = array();
  $api_relationshiptype_api = features_civicrm_api('RelationshipType', 'get');
  foreach ($api_relationshiptype_api['values'] as $relationshiptype) {
    $options[$relationshiptype['name_a_b']] = $relationshiptype['name_a_b'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function relationshiptype_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  foreach ($data as $relationshiptype) {
    $api_relationshiptype_api = features_civicrm_api('RelationshipType', 'get', array('name_a_b' => $relationshiptype));
    if ($api_relationshiptype_api['count'] == 1) {
      $contacttype_a = $api_relationshiptype_api['values'][$api_relationshiptype_api['id']]['contact_type_a'];
      $contacttype_b = $api_relationshiptype_api['values'][$api_relationshiptype_api['id']]['contact_type_b'];
      $export['features']['contacttype'][$contacttype_a] = $contacttype_a;
      $export['features']['contacttype'][$contacttype_b] = $contacttype_b;
      if (isset($api_relationshiptype_api['values'][$api_relationshiptype_api['id']]['contact_sub_type_a'])) {
        $contactsubtype_a = $api_relationshiptype_api['values'][$api_relationshiptype_api['id']]['contact_sub_type_a'];
        $export['features']['contacttype'][$contactsubtype_a] = $contactsubtype_a;
      }
      if (isset($api_relationshiptype_api['values'][$api_relationshiptype_api['id']]['contact_sub_type_b'])) {
        $contactsubtype_b = $api_relationshiptype_api['values'][$api_relationshiptype_api['id']]['contact_sub_type_b'];
        $export['features']['contacttype'][$contactsubtype_b] = $contactsubtype_b;
      }
    }
    $export['features']['relationshiptype'][$relationshiptype] = $relationshiptype;
  }
  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function relationshiptype_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_relationshiptype = array();';
  $code[] = '';
  foreach ($data as $relationshiptype_key => $relationshiptype_value) {
    $relationshiptype_api_result
      = features_civicrm_api('RelationshipType', 'get', array('name_a_b' => $relationshiptype_value));
    if ($relationshiptype_api_result['count'] == 1) {
      $relationshiptype_id = $relationshiptype_api_result['id'];
      $relationshiptype_api_result = $relationshiptype_api_result['values'][$relationshiptype_id];
      $relationshiptype_api_result = relationshiptype_remove_fields($relationshiptype_api_result);
      $relationshiptype_code = features_var_export($relationshiptype_api_result);
      $code[] = "  \$civicrm_relationshiptype['{$relationshiptype_value}'] = {$relationshiptype_code};";
    }
  }
  $code[] = '  return $civicrm_relationshiptype;';
  $code = implode("\n", $code);
  return array('civicrm_default_relationshiptype' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function relationshiptype_features_rebuild($module) {
  relationshiptype_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function relationshiptype_features_revert($module) {
  $civicrm_relationshiptypes = module_invoke($module, 'civicrm_default_relationshiptype');
  foreach ($civicrm_relationshiptypes as $relationshiptype) {
    $existing_relationshiptype = features_civicrm_api('RelationshipType', 'get', array('id' => $relationshiptype['name_a_b']));
    // Merge with code_relationshiptype.
    $merged_relationshiptype = array_merge_recursive_distinct($existing_relationshiptype['values'][$existing_relationshiptype['id']], $relationshiptype);
    $api_relationshiptypes_api = features_civicrm_api('RelationshipType', 'create', $merged_relationshiptype);
  }
}
