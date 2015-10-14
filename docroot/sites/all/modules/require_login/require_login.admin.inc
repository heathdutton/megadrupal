<?php

/**
 * @file
 * Administration menu callbacks for Require Login.
 */

function require_login_admin_settings() {
  $form = array();
  $form['require_login_excluded_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Excluded paths'),
    '#default_value' => variable_get('require_login_excluded_paths'),
    '#description' => t('Users will be able to access these paths without logging in.
      One path per line.'),
  );

  $form['#submit'][] = 'require_login_admin_settings_submit';
  return system_settings_form($form);
}

function require_login_admin_settings_submit($form, &$form_state) {
  $form_state['values']['require_login_excluded_paths'] = trim($form_state['values']['require_login_excluded_paths']);
  variable_set('require_login_excluded_paths', $form_state['values']['require_login_excluded_paths']);
}
