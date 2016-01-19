<?php
/**
 * @file
 * Administrative page code for the webform ftp module.
 */

/**
 * Form callback for "admin/config/content/webform_ftp".
 */
function webform_ftp_admin_settings($form, &$form_state) {
  $form = array();
  $form['webform_ftp_default_time'] = array(
    '#type' => 'select',
    '#title' => t('Webform submission upload to FTP period (default)'),
    '#description' => '',
    '#options' => _webform_ftp_get_ftp_time_options(),
    '#default_value' => variable_get('webform_ftp_default_time', WEBFORM_FTP_NOT_UPLOAD),
    '#description' => t('Period runtime.'),
  );

  $form['webform_ftp_ftp_host'] = array(
    '#type' => 'textfield',
    '#title' => t('HOST'),
    '#default_value' => variable_get('webform_ftp_ftp_host', ''),
    '#description' => t('Computer direction or other device providing data or services that a remote computer can access by means of a network.'),
  );

  $form['webform_ftp_ftp_port'] = array(
    '#type' => 'textfield',
    '#title' => t('PORT'),
    '#default_value' => variable_get('webform_ftp_ftp_port', '22'),
    '#description' => t('Number of port to connect to server. Defaults values: 21 or 22.'),
  );

  $form['webform_ftp_ftp_user'] = array(
    '#type' => 'textfield',
    '#title' => t('USER'),
    '#default_value' => variable_get('webform_ftp_ftp_user', ''),
    '#description' => t('User allowed to connect to the server.'),
  );

  $form['webform_ftp_ftp_pass'] = array(
    '#type' => 'password',
    '#title' => t('PASS'),
    '#post_render' => array('webform_ftp_hash'),
    '#description'   => t('Password of username. If you have already entered your password before, you should leave this field blank, unless you want to change the stored password.'),
  );

  $form['webform_ftp_ftp_path'] = array(
    '#type' => 'textfield',
    '#title' => t('PATH'),
    '#default_value' => variable_get('webform_ftp_ftp_path', ''),
    '#description' => t('Remote location folder'),
  );

  $form = system_settings_form($form);
  $form['#submit'] = array_merge(array('webform_ftp_password'), (array) $form['#submit']);

  return system_settings_form($form);
}

/**
 * Validation for the administrative settings form.
 */
function webform_ftp_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['webform_ftp_ftp_host'] == 1 && $form_state['values']['webform_ftp_ftp_host'] == '') {
    form_set_error('webform_ftp_ftp_host', t('You must enter an HOST'));
  }

  if ($form_state['values']['webform_ftp_ftp_port'] == 1 && $form_state['values']['webform_ftp_ftp_port'] == '') {
    form_set_error('webform_ftp_ftp_port', t('You must enter an PORT'));
  }

  if ($form_state['values']['webform_ftp_ftp_path'] == 1 && $form_state['values']['webform_ftp_ftp_path'] == '') {
    form_set_error('webform_ftp_ftp_path', t('You must enter an PATH'));
  }

  if (empty($form_state['values']['webform_ftp_ftp_user'])) {
    $form_state['values']['webform_ftp_ftp_pass'] = '';
  }
  elseif (empty($form_state['values']['webform_ftp_ftp_pass'])) {
    unset($form_state['values']['webform_ftp_ftp_pass']);
  }
}

/**
 * Encrypt the server pass.
 */
function webform_ftp_password($form, &$form_state) {
  if (!empty($form_state['values']['webform_ftp_ftp_pass'])) {
    $iv = openssl_random_pseudo_bytes(openssl_cipher_iv_length(WEBFORM_FTP_ENCRYPTIONMETHOD));
    $form_state['values']['webform_ftp_ftp_pass'] = openssl_encrypt($form_state['values']['webform_ftp_ftp_pass'], WEBFORM_FTP_ENCRYPTIONMETHOD, WEBFORM_FTP_HASH_KEY, 0, $iv) . ':' . $iv;
  }
}

/**
 * Saves webform configuration.
 */
function webform_ftp_configure_save($form, &$form_state) {
  $ftp_time = $form_state['values']['ftp_time'];
  $nid = $form_state['values']['nid'];

  db_delete('webform_ftp')->condition('nid', $nid)->execute();
  db_insert('webform_ftp')->fields(array('nid' => $nid, 'ftp_time' => $ftp_time))->execute();
}

/**
 * Gets the "ftp time" dropdown options.
 */
function _webform_ftp_get_ftp_time_options() {
  return array(
    WEBFORM_FTP_NOT_UPLOAD => t('Do not upload submissions to ftp'),
    1 * 24 * 60 * 60 => t('Upload submissions after 1 day'),
    7 * 24 * 60 * 60 => t('Upload submissions after @count days', array('@count' => 7)),
    30 * 24 * 60 * 60 => t('Upload submissions after @count days', array('@count' => 30)),
    1 => t('Upload submissions on each cron execution'),
  );
}
