<?php

/**
 * @file
 * Import/Export functionality provided by Uc sermepa module.
 */

/**
 * Export sermepa settings form.
 */
function uc_sermepa_export_settings(&$form_state) {
  $oid = arg(5);
  $setting = uc_sermepa_settings_load($oid);
  drupal_set_title($setting->bankname);
  $code = uc_sermepa_settings_export($setting);
  $lines = substr_count($code, "\n");
  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into another setting using the import function.'),
    '#suffix' => l(t('Back to Sermepa settings page.'), 'admin/store/settings/payment/edit/uc_semepa'),
  );
  return $form;
}

/**
 * Load a single bank setting.
 */
function uc_sermepa_settings_load($oid) {
  if (module_exists('ctools')) {
    ctools_include('export');
    $result = ctools_export_load_object('uc_sermepa_settings', 'names', array($oid));
    if (isset($result[$oid])) {
      return $result[$oid];
    }
  }
}

/**
 * Export a sermepa setting.
 */
function uc_sermepa_settings_export($obj, $indent = '') {
  if (module_exists('ctools')) {
    ctools_include('export');
    $output = ctools_export_object('uc_sermepa_settings', $obj, $indent);
    return $output;
  }
}

/**
 * Import tax rate.
 */
function uc_sermepa_import_settings() {
  $form = array();
  $form['import'] = array(
    '#type' => 'textarea',
    '#default_value' => '',
    '#rows' => 10,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import')
  );
  return $form;
}

/**
 * Validate tax import.
 */
function uc_sermepa_import_settings_validate($form, &$form_state) {
  ob_start();
  eval($form_state['values']['import']);
  ob_end_clean();
  if (empty($setting) || !is_object($setting) || empty($setting->bankcode)) {
    form_set_error('import', t('The submitted bank code could not be interperated.'));
  }
  elseif (uc_sermepa_get_bank($setting->bankcode)) {
    form_set_error('import', t('A bank with the same code already exists.'));
  }
  else {
    //Pass the parsed object on to the submit handler.
    $form_state['values']['import_parsed'] = $setting;
  }
}

/**
 * Submit tax import.
 */
function uc_sermepa_import_settings_submit($form, &$form_state) {
  $saved = uc_sermepa_save_bank($form_state['values']['import_parsed']);
  if (!$saved) {
    drupal_set_message(t('Failed to import the bank.'), 'warning');
  }
  else {
    drupal_set_message(t('Bank %name imported', array('%name' => $form_state['values']['import_parsed']->bankname)));
  }
  $form_state['redirect'] = 'admin/store/settings/payment/edit/uc_semepa';
}
