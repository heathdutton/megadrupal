<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Implements hook_features_export_options().
 */
function setting_features_export_options() {
  $options = array();
  $setting_getfields = features_civicrm_api('setting', 'getfields');
  $setting_getfields = $setting_getfields['values'];
  $setting_get = features_civicrm_api('setting', 'get');
  $setting_id = $setting_get['id'];
  $setting_get = $setting_get['values'][$setting_id];
  foreach ($setting_get as $setting_key => $setting_value) {
    if (isset($setting_getfields[$setting_key])) {
      $setting = $setting_getfields[$setting_key];
      if (isset($setting['title']) && $setting['title'] != '') {
        $options[$setting['name']] = $setting['title'] . ' (' . $setting['name'] . ')';
      }
      else {
        $options[$setting['name']] = $setting['name'];
      }
    }
  }
  // Remove all directories, since the're absolute and site-specific.
  unset($options['customCSSURL']);
  unset($options['uploadDir']);
  unset($options['imageUploadDir']);
  unset($options['customFileUploadDir']);
  unset($options['customTemplateDir']);
  unset($options['customPHPPathDir']);
  unset($options['extensionsDir']);
  unset($options['userFrameworkResourceURL']);
  unset($options['imageUploadURL']);
  return $options;
}

/**
 * Implements hook_features_export().
 */
function setting_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  foreach ($data as $setting) {
    $export['features']['setting'][$setting] = $setting;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function setting_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_setting = array();';
  $code[] = '';
  $setting_get = features_civicrm_api('Setting', 'get');
  $setting_id = $setting_get['id'];
  $setting_get = $setting_get['values'][$setting_id];
  foreach ($data as $setting) {
    if (isset($setting_get[$setting])) {
      $setting_code = features_var_export($setting_get[$setting]);
      $code[] = "  \$civicrm_setting['{$setting}'] = {$setting_code};";
    }
  }
  $code[] = '  return $civicrm_setting;';
  $code = implode("\n", $code);
  return array('civicrm_default_setting' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function setting_features_rebuild($module) {
  setting_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function setting_features_revert($module) {
  $code_settings = module_invoke($module, 'civicrm_default_setting');
  $code_settings += array('domain_id' => 'all');
  // Remove lcMessages from create: see https://drupal.org/node/2008816.
  if (isset($code_settings['lcMessages'])) {
    unset($code_settings['lcMessages']);
    watchdog(
    'features_civicrm',
    'The CiviCRM (Setting, Create, lcMessage) API function is not implemented. You need to revert the language manually. ',
    array(),
    WATCHDOG_ERROR);
    drupal_set_message(
      t('The CiviCRM (Setting, Create, lcMessage) API function is not implemented. You need to revert the language manually. '));
  }
  $settings_api = features_civicrm_api('Setting', 'create', $code_settings);
}
