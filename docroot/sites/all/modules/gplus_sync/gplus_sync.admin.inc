<?php

require_once 'gplus_sync.constants.inc';

/**
 * gplus_sync_settings_form().
 * gplus_sync_settings_form_submit().
 */
function gplus_sync_settings_form($form, &$form_state) {
// Google+ Access parameters
  $form['gplus_access_params'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google+ Access parameters'),
  );
  $form['gplus_access_params']['client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('gplus_sync_client_id'),
    '#description' => t('Example: !example', array('!example' => '012345678901-abcdefghijklmnqrstuvwxyz01234567.apps.googleusercontent.com')),
    '#required' => TRUE,
  );
  $form['gplus_access_params']['client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#default_value' => variable_get('gplus_sync_client_secret'),
    '#description' => t('Example: !example', array('!example' => '0123-456789abcdefghijklm')),
    '#required' => TRUE,
  );
// Google+ Login parameters
  $form['gplus_login_params'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google+ Login parameters'),
  );
  $form['gplus_login_params']['available_domains'] = array(
    '#title' => t('Available domains'),
    '#type' => 'textarea',
    '#default_value' => variable_get('gplus_sync_available_domains'),
    '#description' => t('This is the filter by domain name in user email address.').GPLUS_SYNC_HTML_BR.
                      t('Write each value in new line.').GPLUS_SYNC_HTML_BR.
                      t('If empty - all domain names will be allowed.'),
  );
  $form['gplus_login_params']['is_create_new_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new user'),
    '#default_value' => variable_get('gplus_sync_is_create_new_user', TRUE),
    '#description' => t('If this option is enabled then new Drupal profile will be created for new user.').GPLUS_SYNC_HTML_BR.
                      t('If this option is disabled then user without exist Drupal profile cannot login to the site via Google+ button.')
  );
/* Actions */
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function gplus_sync_settings_form_submit($form, &$form_state) {
  variable_set('gplus_sync_client_id',          $form_state['values']['client_id']);
  variable_set('gplus_sync_client_secret',      $form_state['values']['client_secret']);
  variable_set('gplus_sync_available_domains',  $form_state['values']['available_domains']);
  variable_set('gplus_sync_is_create_new_user', $form_state['values']['is_create_new_user']);
  drupal_set_message(t('Settings was saved.'));
}

