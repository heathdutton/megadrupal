<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Implements hook_features_export_options().
 */
function constant_features_export_options() {
  $options = array();
  $api_constant_api = features_civicrm_api('constant', 'getfields');
  foreach ($api_constant_api['values']['name'] as $constant_key => $constant_value) {
    $options[$constant_value] = $constant_value;
  }
  unset($options['1']);
  unset($options['addressee']);
  unset($options['emailGreeting']);
  unset($options['postalGreeting']);
  unset($options['stateProvinceForCountry']);
  return $options;
}

/**
 * Implements hook_features_export().
 */
function constant_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  foreach ($data as $constant) {
    $export['features']['constant'][$constant] = $constant;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function constant_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_constant = array();';
  $code[] = '';
  foreach ($data as $constant_key => $constant_value) {
    $constant_api_result = features_civicrm_api('Constant', 'get', array('name' => $constant_value));
    if ($constant_api_result['is_error'] == 0) {
      $constant_api_result = $constant_api_result['values'];
      if (count(features_var_export($constant_api_result)) > 0) {
        $constant_code = features_var_export($constant_api_result);
        $code[] = "  \$civicrm_constant['{$constant_value}'] = {$constant_code};";
      }
    }
  }
  $code[] = '  return $civicrm_constant;';
  $code = implode("\n", $code);
  return array('civicrm_default_constant' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function constant_features_rebuild($module) {
  constant_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function constant_features_revert($module) {
  watchdog(
    'features_civicrm',
    'The CiviCRM (Constant, Create)API function is not implemented. So reverting needs to happen manually. ',
    array(),
    WATCHDOG_ERROR);
  drupal_set_message(
    t('The CiviCRM (Constant, Create)API function is not implemented. So reverting needs to happen manually. '));
}
