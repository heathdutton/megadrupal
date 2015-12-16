<?php

/**
 * @file
 * Admin callbacks and functions for the SkaFlash module.
 */


/**
 * Admin page callback for SkaFlash configuration.
 */
function skaflash_admin_form($form, &$form_state) {
  // Access log settings.
  $form['push_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Push notifications'),
    '#collapsible' => FALSE,
  );
  $form['push_notifications']['skaflash_endpoint_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#required' => TRUE,
    '#default_value' => variable_get('skaflash_endpoint_path', SKAFLASH_ENDPOINT_PATH),
    '#description' => '<p>' . t('The path used to listen for push notifications from the SkaFlash API.') . '</p>' .
      '<p>' . t('Use the following URL when configuring API settings in SkaFlash: !url', array(
      '!url' => '<strong>' . format_string('<span id="endpoint-base">@base_url</span>/<span id="endpoint-path">@path</span>', array(
        '@base_url' => $GLOBALS['base_url'],
        '@path' => variable_get('skaflash_endpoint_path', SKAFLASH_ENDPOINT_PATH),
      )) . '</strong>',
    )) . '</p>',
  );

  // Asset attachments for this form.
  $mod_path = drupal_get_path('module', 'skaflash');
  $form['#attached']['js'] = array($mod_path . '/misc/skaflash.admin.js');

  // Attach validation handler.
  $form['#validate'] = array('skaflash_admin_form_validate');

  // Attach a submit handler AFTER the variable save to trigger a menu rebuild.
  $renderable = system_settings_form($form);
  $renderable['#submit'][] = 'skaflash_admin_form_submit';
  return $renderable;
}


/**
 * Validation handler for the skaflash admin configuration form.
 */
function skaflash_admin_form_validate($form, &$form_state) {
  // Ensure that the path is not already in use.
  $new_path = $form_state['input']['skaflash_endpoint_path'];
  $old_path = variable_get('skaflash_endpoint_path', SKAFLASH_ENDPOINT_PATH);

  // Assume that existing configurations are already valid.
  if ($new_path != $old_path) {
    // Ensure that the path does not already exist in the system.
    if (drupal_valid_path($new_path, FALSE)) {
      form_set_error('skaflash_endpoint_path', t('The path %path is already in use.', array('%path' => $new_path)));
    }
  }
}


/**
 * Submit handler for the skaflash admin configuration form.
 */
function skaflash_admin_form_submit($form, &$form_state) {
  // Ensure that the endpoint callback is up-to-date.
  menu_rebuild();
}
