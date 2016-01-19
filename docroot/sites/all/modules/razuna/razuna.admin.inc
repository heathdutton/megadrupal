<?php
/**
 * @file
 * Contains all admin pages and settings.
 */

/**
 * Admin setting form.
 */
function razuna_admin_settings($form_state) {
  $form['razuna'] = array(
    '#type' => 'fieldset',
    '#title' => t('Razuna'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['razuna']['razuna_hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Razuna Hostname'),
    '#default_value' => variable_get('razuna_hostname', ''),
    '#required' => TRUE,
  );
  $form['razuna']['razuna_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Razuna API Kay'),
    '#default_value' => variable_get('razuna_api_key', ''),
    '#required' => TRUE,
  );
  $form['razuna']['razuna_folderid'] = array(
    '#type' => 'textfield',
    '#title' => t('Razuna Folder Id'),
    '#default_value' => variable_get('razuna_folderid', ''),
    '#required' => TRUE,
  );
  $form['razuna']['razuna_emailid'] = array(
    '#type' => 'textfield',
    '#title' => t('Razuna Email Id'),
    '#default_value' => variable_get('razuna_emailid', ''),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Validate Admin setting form.
 */
function razuna_admin_settings_validate($form, &$form_state) {
  $razuna_hostname = $form_state['values']['razuna_hostname'];
  // Remove index.cfm as some may add this but this is only needed while
  // sending the file to razuna but not required for getting the files from
  // razuna. This index.cfm will be added manually at the time of sending
  // files to razuna.
  $razuna_hostname = str_replace('/index.cfm', "", $razuna_hostname);
  if (!filter_var($razuna_hostname, FILTER_VALIDATE_URL)) {
    form_set_error('razuna', t('Please enter valid URL.'));
  }
  else {
    form_set_value($form['razuna']['razuna_hostname'], $razuna_hostname, $form_state);
  }
}
