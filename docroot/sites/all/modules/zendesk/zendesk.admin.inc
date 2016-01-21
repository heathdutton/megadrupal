<?php
/**
 * @file
 * Admin page callback file for the Zendesk module.
 */

/**
 * Zendesk configuration form.
 */
function zendesk_configuration_form() {

  $form['zendesk']['zendesk_conf'] = array(
    '#type' => 'fieldset',
    '#title' => 'General configuration',
  );

  $form['zendesk']['zendesk_conf']['zendesk_url'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => 'The url of your zendesk support page (e.g. http://yourdomain.zendesk.com).',
    '#default_value' => variable_get('zendesk_url', 'http://yourdomain.zendesk.com'),
  );

  $form['zendesk']['zendesk_conf']['zendesk_api_token'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => 'The zendesk API token.',
    '#default_value' => variable_get('zendesk_api_token', 'Zendesk secret key'),
    '#suffix' => t('Use the http://www.yourdomain.com/agent/#/admin/channels page in your zendesk configuration page. (Go to Account -> Channels'),
  );

  $form['zendesk']['zendesk_conf']['zendesk_secret_key'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => 'The zendesk SSO secret key.',
    '#default_value' => variable_get('zendesk_secret_key', 'Zendesk secret key'),
    '#suffix' => t('Use the http://www.yourdomain.com/agent/#/admin/security page in your zendesk configuration page. (Go to Account -> Security'),
  );

  $form['zendesk']['zendesk_api'] = array(
    '#type' => 'fieldset',
    '#required' => TRUE,
    '#title' => 'Zendesk API configuration',
    '#description' => t('The credentials of the administrator in order to use api calls.'),
  );

  $form['zendesk']['zendesk_api']['zendesk_api_sync_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Synchronize the Drupal users with Zendesk'),
    '#default_value' => variable_get('zendesk_api_sync_users', 0),
    '#description' => t('When accounts are created in your drupal site, updated and delete, push these changes to Zendesk.'),
  );

  $form['zendesk']['zendesk_api']['zendesk_api_group'] = array(
    '#type' => 'textfield',
    '#title' => t('API group'),
    '#default_value' => variable_get('zendesk_api_group', ''),
    '#description' => t('this is simple the subdomain of your zendesk page: if your zendesk is http://subdomain.zendesk.com, then you have to fil in "subdomain".'),
  );

  $form['zendesk']['zendesk_api']['zendesk_api_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail address of the API user'),
    '#default_value' => variable_get('zendesk_api_mail', ''),
    '#description' => t('This is typically the mail address of the zendesk admin account'),
  );

  $form['zendesk']['zendesk_api']['zendesk_authed_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create user as verified users'),
    '#default_value' => variable_get('zendesk_authed_user', FALSE),
    '#description' => t('Check this is you want to create user as already verified users.'),
  );

  // Role-based visibility settings.
  $result = db_query('SELECT rid, name FROM {role} WHERE rid != 1 ORDER BY name');
  $role_options = array();
  foreach ($result as $role) {
    $role_options[$role->rid] = $role->name;
  }

  $form['zendesk']['zendesk_permissions'] = array(
    '#type' => 'fieldset',
    '#description' => t('Restrict access to zendesk based on user roles. These rules will apply for both user synchronization as remote authentication.'),
  );

  $form['zendesk']['zendesk_permissions']['zendesk_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Authenticate only for specific roles'),
    '#default_value' => variable_get('zendesk_roles', array()),
    '#options' => $role_options,
    '#description' => t('Select which roles may be authenticated for zendesk. If you select no roles, all authenticated drupal users will be authenticated for Zendesk.'),
  );

  $form['zendesk']['zendesk_permissions']['zendesk_no_permission_page'] = array(
    '#type' => 'textfield',
    '#title' => t('No permission page'),
    '#default_value' => variable_get('zendesk_no_permission_page', ''),
    '#description' => t('To what pages do you want to redirect user that have no permission to access Zendesk.'),
  );

  return system_settings_form($form);
}
