<?php

/**
 * @file
 * Administrative page callbacks for the Listserv module.
 */

/**
 * Listserv admin configuration form.
 */
function listserv_admin_config_form($form, &$form_state) {
  $form['listserv_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Listserv email'),
    '#default_value' => variable_get('listserv_email', LISTSERV_EMAIL),
    '#description' => t('Set the email address of the listserv list.'),
  );
  $form['listserv_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Listserv default name'),
    '#default_value' => variable_get('listserv_default', LISTSERV_DEFAULT),
    '#description' => t('Set the default name of the listserv.'),
  );

  return system_settings_form($form);
}

/**
 * Listserv admin config form validate handler.
 */
function listserv_admin_config_form_validate($form, &$form_state) {
  $default_email = $form_state['values']['listserv_email'];
  // Verify that the email address is valid.
  if (!valid_email_address($default_email)) {
    form_set_error('default_email', t('You must provide a valid email address.'));
  }
}
