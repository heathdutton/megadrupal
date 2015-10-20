<?php

/**
 * @file
 * Administration menu callbacks for Privatemsg unread.
 */

function privatemsg_unread_admin_settings() {
  $form = array();
  $form['privatemsg_unread_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Unread messages path'),
    '#default_value' => variable_get('privatemsg_unread_path'),
    '#description' => t("The path to view unread private messages."),
    '#required' => TRUE,
  );

  $form['#submit'][] = 'privatemsg_unread_admin_settings_submit';
  return system_settings_form($form);
}

function privatemsg_unread_admin_settings_validate($form, &$form_state) {
	$saved_path = variable_get('privatemsg_unread_path');
  $path = $form_state['values']['privatemsg_unread_path'];
  $path_changed = ($path == $saved_path) ? FALSE : TRUE;
  if ($path_changed) {
    // A different path than saved is entered.
    $path_is_valid = _privatemsg_unread_check_path($path);
    if (!$path_is_valid) {
      form_set_error('privatemsg_unread_path', t('The path is already in use.'));
    }
  }
  else {
    drupal_set_message('No changes to save.');
  }
}

function privatemsg_unread_admin_settings_submit($form, &$form_state) {
  variable_set('privatemsg_unread_path', $form_state['values']['privatemsg_unread_path']);
  drupal_flush_all_caches();
}
