<?php
/**
 * @file
 * deploy_addon.deploy_endpoints.inc
 */

/**
 * Implements hook_deploy_endpoints_default().
 */
function deploy_addon_deploy_endpoints_default() {
  $export = array();

  // Get next endpoint password and decrypt if needed.
  $password = variable_get('deploy_addon_next_pass');
  if ($password && variable_get('deploy_addon_next_pass_encrypt')) {
    $password = decrypt($password);
  }

  $endpoint = new DeployEndpoint();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 1;
  $endpoint->name = 'next';
  $endpoint->title = 'Next';
  $endpoint->description = 'Endpoint for deployments.';
  $endpoint->debug = variable_get('deploy_addon_debug');
  $endpoint->authenticator_plugin = 'DeployAuthenticatorSession';
  $endpoint->authenticator_config = array(
    'username' => variable_get('deploy_addon_next_user'),
    'password' => $password,
  );
  $endpoint->service_plugin = 'DeployServiceRestJSON';
  $endpoint->service_config = array(
    'url' => variable_get('deploy_addon_next_url'),
  );
  $export['next'] = $endpoint;

  return $export;
}
