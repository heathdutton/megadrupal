<?php

/**
 * Settings page.
 */
function oauth3legged_client_admin($form, &$form_state) {
  $form = array();

  $default = variable_get('oauth3legged_client', _oauth3legged_client_default());

  $form['oauth3legged_client'] = array(
    '#tree' => TRUE,
  );

  $form['oauth3legged_client']['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#description' => t('Field names in user entity.'),
    '#collapsible' => FALSE,
  );

  $form['oauth3legged_client']['fields']['token_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Token key'),
    '#default_value' => $default['fields']['token_key'],
  );

  $form['oauth3legged_client']['fields']['token_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Token secret'),
    '#default_value' => $default['fields']['token_secret'],
  );

  $form['oauth3legged_client']['fields']['token_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Token type'),
    '#default_value' => $default['fields']['token_type'],
  );

  $form['oauth3legged_client']['provider'] = array(
    '#type' => 'fieldset',
    '#title' => t('Provider'),
    '#collapsible' => FALSE,
  );

  $form['oauth3legged_client']['provider']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('No tailing slash, i.e. http://oapi.mobingi.com'),
  '#default_value' => $default['provider']['url'],
  );

  $form['oauth3legged_client']['provider']['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint'),
    '#description' => t('No tailing slash, i.e. api/v1'),
    '#default_value' => $default['provider']['endpoint'],
  );

  $form['oauth3legged_client']['provider']['consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Key'),
    '#default_value' => $default['provider']['consumer_key'],
  );

  $form['oauth3legged_client']['provider']['consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Secret'),
    '#default_value' => $default['provider']['consumer_secret'],
  );

  $form['oauth3legged_client']['provider']['uri_schema'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom URL schema'),
    '#description' => t('Should config it both here and ios client or others, E.G. dios://'),
    '#default_value' => $default['provider']['uri_schema'],
  );
  // validation
  $form['#validate'][] = 'oauth3legged_client_admin_validate';

  return system_settings_form($form);
}

/**
 * Settings form validation.
 */
function oauth3legged_client_admin_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if (!valid_url($values['oauth3legged_client']['provider']['url'], TRUE)) {
    form_error($form['oauth3legged_client']['provider']['url'], t('Please input valid URL.'));
  }
}

/**
 * Example page.
 */
function oauth3legged_client_example() {
  global $base_url;
  $form = array();

  $form['create_user'] = array(
    '#markup' => l(t('Create Mobingi user'), $base_url . "/admin/config/services/oauth3legged_client/example/create_user"),
  );

  return $form;
}
