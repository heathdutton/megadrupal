<?php

/**
 * @file
 * Integrates passwd only with logintoboggan.
 */

/**
 * Settings form to set the PassWd only user.
 *
 * @see passwd_only_admin_submit()
 */
function passwd_only_logintoboggan_admin() {

  $form = array();

  $form['passwd_only_logintoboggan_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#default_value' => variable_get('passwd_only_logintoboggan_link_text', t('Log in with PassWd only')),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return system_settings_form($form);
}
