<?php

/**
 * @file
 * Administration UI for acrypt.
 */

/**
 * Acrypt configuration form.
 */
function acrypt_gpg_ui_admin($form, &$form_state) {
  $form['acrypt_gpg_public_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste here the GnuPG public key. Do not enter private key, because it will be a security hole.'),
    '#default_value' => variable_get('acrypt_gpg_public_key', NULL),
  );

  return system_settings_form($form);
}
