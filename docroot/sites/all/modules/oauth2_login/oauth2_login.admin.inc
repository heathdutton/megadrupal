<?php
/**
 * @file
 * Administrative pages for the oauth2_login module.
 */

/**
 * Admin settings for the module oauth2_login.
 *
 * @return
 *   An array containing form items to place on the module settings page.
 */
function oauth2_login_admin_settings() {
  $form = [
    '#type'  => 'fieldset',
    '#title' => t('OAuth2 Login Settings'),

    'oauth2_login_enabled' => [
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => variable_get('oauth2_login_enabled', FALSE),
      '#description' => t('Check to enable OAuth2 Login.'),
    ],

    'oauth2_login_oauth2_server' => [
      '#type' => 'textfield',
      '#title' => t('Server URL'),
      '#default_value' => variable_get('oauth2_login_oauth2_server', ''),
      '#description' => t('The URI of the OAuth2 Login Provider.'),
      '#states' => ['visible' => [':input[name="oauth2_login_enabled"]' => ['checked' => TRUE]]],
    ],

    'oauth2_login_client_id' => [
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#default_value' => variable_get('oauth2_login_client_id', ''),
      '#description' => t('The Client ID that is registered on the OAuth2 server.'),
      '#states' => ['visible' => [':input[name="oauth2_login_enabled"]' => ['checked' => TRUE]]],
    ],

    'oauth2_login_client_secret' => [
      '#type' => 'textfield',
      '#title' => t('Client secret'),
      '#default_value' => variable_get('oauth2_login_client_secret', ''),
      '#description' => t('The Client secret that is registered on the OAuth2 server.'),
      '#states' => ['visible' => [':input[name="oauth2_login_enabled"]' => ['checked' => TRUE]]],
    ],

    'testing' => [
      '#type'  => 'fieldset',
      '#title' => t('Settings Useful for Development, Testing and Debug'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => ['visible' => [':input[name="oauth2_login_enabled"]' => ['checked' => TRUE]]],

      'oauth2_login_skipssl' => [
        '#type' => 'checkbox',
        '#title' => t('Skip SSL'),
        '#description' => t('Skip checking the SSL certificate.'),
        '#default_value' => variable_get('oauth2_login_skipssl', FALSE),
      ],

      'oauth2_login_proxy' => [
        '#type' => 'textfield',
        '#title' => t('HTTP Proxy'),
        '#description' => t('Use an HTTP proxy agent (like <a href="http://mitmproxy.org" target="_blank">http://mitmproxy.org</a>).'),
        '#default_value' => variable_get('oauth2_login_proxy', ''),
      ],
    ],
  ];

  $form = system_settings_form($form);
  $form['#submit'][] = 'oauth2_login_admin_set_settings';
  $form['#submit'][] = 'oauth2_login_call_hook_enabled';

  return $form;
}

/**
 * Synchronize settings with hybridauth_DrupalOAuth2.
 */
function oauth2_login_admin_set_settings() {
  $oauth2_server = variable_get('oauth2_login_oauth2_server', '');
  $client_id = variable_get('oauth2_login_client_id', '');
  $client_secret = variable_get('oauth2_login_client_secret', '');
  $skipssl = variable_get('oauth2_login_skipssl', FALSE);
  $proxy = variable_get('oauth2_login_proxy', '');

  variable_set('hybridauth_provider_DrupalOAuth2_oauth2_server', $oauth2_server);
  variable_set('hybridauth_provider_DrupalOAuth2_keys_id', $client_id);
  variable_set('hybridauth_provider_DrupalOAuth2_keys_secret', $client_secret);
  variable_set('hybridauth_provider_DrupalOAuth2_skipssl', $skipssl);
  variable_set('hybridauth_provider_DrupalOAuth2_proxy', $proxy);
}

/**
 * Call hook_oauth2_login_enabled() for all modules.
 */
function oauth2_login_call_hook_enabled() {
  $enabled = variable_get('oauth2_login_enabled', FALSE);
  module_invoke_all('oauth2_login_enabled', $enabled);
}
