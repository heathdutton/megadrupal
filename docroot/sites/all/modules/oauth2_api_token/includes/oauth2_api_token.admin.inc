<?php
/**
 * @file
 * Administrative functionality for the OAuth2 API Token module.
 */

/**
 * Configuration form.
 */
function oauth2_api_token_config_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'module', 'includes/oauth2_api_token.admin');

  $server_options = array();
  foreach (entity_load('oauth2_server') as $server) {
    $server_options[$server->name] = $server->label;
  }
  $form['oauth2_api_token_server'] = array(
    '#title' => t('Server'),
    '#type' => 'select',
    '#options' => $server_options,
    '#default_value' => variable_get('oauth2_api_token_server'),
    '#required' => TRUE,
    '#description' => t('The OAuth2 server with which API tokens will be used.'),
  );

  if (!empty($form_state['oauth2_api_token_server_configured'])) {
    $server_name = $form_state['values']['oauth2_api_token_server'];
    $scopes = oauth2_server_entity_load_by_properties('oauth2_server_scope', array(
      'server' => $server_name,
    ));
    $scope_options = array();
    foreach ($scopes as $scope) {
      $scope_options[$scope->name] = $scope->name . ': ' . $scope->description;
    }
    if ($scope_options) {
      $form['oauth2_api_token_scopes'] = array(
        '#title' => t('Available scopes'),
        '#type' => 'checkboxes',
        '#required' => FALSE,
        '#options' => $scope_options,
        '#default_value' => variable_get('oauth2_api_token_scopes', array('account')),
        '#description' => t('The scopes that users may add to their API tokens.'),
      );
    }
    else {
      $form['oauth2_api_token_scopes'] = array(
        '#type' => 'item',
        '#title' => t('Scopes'),
        '#markup' => t('This server does not have any scopes defined.'),
      );
    }
  }

  $form = system_settings_form($form);
  if (empty($form_state['oauth2_api_token_server_configured'])) {
    $form['actions']['submit']['#value'] = t('Continue');
  }

  $form['#submit'][] = 'oauth2_api_token_config_form_submit';

  return $form;
}

/**
 * Validation for the configuration form.
 */
function oauth2_api_token_config_form_validate(&$form, &$form_state) {
  if (empty($form_state['oauth2_api_token_server_configured']) && !empty($form_state['values']['oauth2_api_token_server'])) {
    $form_state['rebuild'] = TRUE;
    $form_state['oauth2_api_token_server_configured'] = TRUE;
  }
}

/**
 * Submit callback for the configuration form.
 *
 * The submit callback system_settings_form_submit() will also run for this
 * form.
 */
function oauth2_api_token_config_form_submit(&$form, &$form_state) {
  $server_name = $form_state['values']['oauth2_api_token_server'];
  oauth2_api_token_create_client($server_name);
}
