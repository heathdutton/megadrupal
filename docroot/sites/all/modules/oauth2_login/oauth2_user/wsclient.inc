<?php
/**
 * @file
 * Define the web service client that gets the oauth2 user profile.
 */

/**
 * Implements hook_default_wsclient_service().
 *
 * Defines web service description for getting the remote (oauth2) user profile.
 * This hook is invoked when web service description is loaded like this:
 *     $oauth2 = wsclient_service_load('oauth2');
 *     $oauth2_user = $oauth2->user_profile();
 */
function oauth2_user_default_wsclient_service() {
  $service_name = 'oauth2';

  // Get oauth2_client settings.
  $server_url = variable_get('oauth2_login_oauth2_server', '');
  $client_id = variable_get('oauth2_login_client_id', '');
  $client_secret = variable_get('oauth2_login_client_secret', '');

  $token_endpoint = $server_url . '/oauth2/token';
  $authorization_endpoint = $server_url . '/oauth2/authorize';
  $redirect_uri = url('oauth2/authorized', ['absolute' => TRUE]);

  // Create a new service description.
  $service = new WSClientServiceDescription();
  $service->name = $service_name;
  $service->label = 'OAuth2 User Profile';
  $service->type = 'rest';
  $service->url = $server_url . '/oauth2/';

  $service->settings['authentication']['oauth2'] = oauth2_login_get_oauth2_settings();
  $service->settings += oauth2_user_wsclient_dev_settings();
  $service->operations = [
    'user_profile' => [
      'label' => 'user_profile',
      'url' => 'user/profile',
      'type' => 'POST',
      'data' => 'params',
      'parameter' => [
        'params' => ['type' => 'array'],
      ],
    ],
  ];

  $services[$service_name] = $service;
  return $services;
}

/**
 * Set curl options for development, testing and debug.
 */
function oauth2_user_wsclient_dev_settings() {
  $skipssl = variable_get('oauth2_login_skipssl', TRUE);
  $proxy = variable_get('oauth2_login_proxy', '');

  $dev_settings = array();
  if ($skipssl) {
    // Skip checking the SSL certificate, for testing.
    $dev_settings['curl options'] = [
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_SSL_VERIFYHOST => FALSE,
    ];
  }
  if ($proxy) {
    $dev_settings['curl options'][CURLOPT_PROXY] = $proxy;
  }
  return $dev_settings;
}
