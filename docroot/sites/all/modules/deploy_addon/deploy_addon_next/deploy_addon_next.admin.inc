<?php
/**
 * @file
 * Administration functions for the Deploy Add-on Next module.
 */

/**
 * Menu callback for admin form.
 */
function deploy_addon_next_admin_form($form, &$form_state) {
  $form = array();

  $encrypt = (module_exists('encrypt') ? TRUE : FALSE);

  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
  );

  $form['configuration']['deploy_addon_next_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Next Endpoint URL'),
    '#default_value' => variable_get('deploy_addon_next_url', NULL),
  );

  $form['configuration']['deploy_addon_next_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Next Endpoint Username'),
    '#default_value' => variable_get('deploy_addon_next_user', NULL),
  );

  $form['configuration']['deploy_addon_next_pass'] = array(
    '#type' => 'password',
    '#title' => t('Next Endpoint Password'),
    '#description' => t('You must have the Encrypt module enabled to enter a password. Otherwise add this to a $conf variable in your settings.php file.'),
    '#disabled' => ($encrypt ? FALSE : TRUE),
  );

  $form['configuration']['deploy_addon_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#default_value' => variable_get('deploy_addon_debug'),
  );

  return system_settings_form($form);
}

/**
 * Validation handler for admin form.
 */
function deploy_addon_next_admin_form_validate($form, &$form_state) {
  // If Encrypt is enabled and a password is set, update it.
  if (module_exists('encrypt') && !empty($form_state['values']['deploy_addon_next_pass'])) {
    $form_state['values']['deploy_addon_next_pass'] = encrypt($form_state['values']['deploy_addon_next_pass']);
    $form_state['values']['deploy_addon_next_pass_encrypt'] = 1;
  }
  else {
    // Remove password.
    $form_state['values']['deploy_addon_next_pass'] = '';
    $form_state['values']['deploy_addon_next_pass_encrypt'] = 0;
  }
}
