<?php
/**
 * @file
 *  Gitolite admin pages.
 */

/**
 * Confirmation form for server sync action.
 */
function gitolite_server_sync_confirm_form($form, &$form_state, $git_server) {
  return confirm_form($form, 'Synchronise all users and SSH keys to this gitolite server?', "admin/config/development/git/servers", "This may take a while.", 'Synchronise', 'Cancel');
}

/**
 * Submit handler for server sync action.
 */
function gitolite_server_sync_confirm_form_submit($form, $form_state) {
  if (!empty($form_state['values']['confirm'])) {
    $git_server = reset($form_state['build_info']['args']);
    gitolite_server_sync($git_server);
    drupal_goto('admin/config/development/git/servers');
  }
}

/**
 * Confirmation form for server sync action.
 */
function gitolite_server_install_confirm_form($form, &$form_state, $git_server) {
  return confirm_form($form, 'Install managed configuration on this gitolite server?', "admin/config/development/git/servers", "This may take a while.", 'Install', 'Cancel');
}

/**
 * Submit handler for server sync action.
 */
function gitolite_server_install_confirm_form_submit($form, $form_state) {
  if (!empty($form_state['values']['confirm'])) {
    try {
      $git_server = reset($form_state['build_info']['args']);
      gitolite_client($git_server->user, $git_server->host)->install();
      gitolite_setup_permissions($git_server);
      drupal_goto('admin/config/development/git/servers');
    }
    catch (Exception $e) {
      watchdog_exception('gitolite', $e);
      drupal_set_message(t('Failed to install gitolite'), 'error');
      return FALSE;
    }
  }
}
