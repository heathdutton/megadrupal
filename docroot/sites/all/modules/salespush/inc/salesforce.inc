<?php

/**
 * @file
 * Helpers for handling with salesforce via API.
 * Initially all routines tied to Enterprise wsdl and API.
 */

/**
 * Upserting Campaign to SalesForce.
 *
 * @param SforceEnterpriseClient $client
 *   Enterprise Client instance.
 * @param array $campaigns
 *   Array of campaigns stdClass objects. See example below.
 *
 * @return array
 *   Associative array with status and exception as key = >value if any.
 */
function salespush_upsert_campaign($client, $campaigns) {
  // Example for campaign array of objects.
  // $campaigns[0] = new stdClass();
  // $campaigns[0]->Name = 'DemoXXXCampaign';
  // $campaigns[0]->Type = 'Demo';
  // $campaigns[0]->Id = '701L000000075ieIAA';
  // Array should start from 0 index. Do not use random indexes here.

  $result = array(
    'status' => array(),
    'exception' => ''
  );

  try {
    $result['status'] = $client->upsert('Id', $campaigns, 'Campaign');
    watchdog('SF campaigns ' . $environment, 'Created campaigns: ' . count(array_keys($result)));
  }
  catch (Exception $e) {
    $result['exception'] = 'Exception: ' . var_export($e, TRUE);
  }

  return $result;
}

/**
 * SforceEnterpriseClient initialization.
 *
 * @param string $username
 *   Login name to salesforce.
 * @param string $password
 *   Concatenated string password.security_token.
 * @param string $wsdl
 *   Local path to wsdl file.
 *
 * @return array
 *   Associated array of client instance, result, session and location data.
 */
function salespush_get_client($username, $password, $wsdl) {
  $client = & drupal_static(__FUNCTION__ . $username . $password . $wsdl);
  if (isset($client)) {
    return $client;
  }

  require_once __DIR__ . '/../vendor/developerforce/force.com-toolkit-for-php/soapclient/SforceEnterpriseClient.php';
  $sclient = new SforceEnterpriseClient();
  $sclient->createConnection($wsdl);

  $login_result = $sclient->login($username, $password);
  $session_id = $sclient->getSessionId();
  $location = $sclient->getLocation();

  $client = array(
    'login_result' => $login_result,
    'session_id' => $session_id,
    'location' => $location,
    'client' => $sclient
  );

  return $client;
}

/**
 * Get path to wsdl depending from the environment.
 *
 * @param string $environment
 *   Environment name.
 *
 * @return string
 *   Full path to wsdl xml file.
 */
function salespush_get_wsdl($environment = 'sandbox') {
  if ($environment == 'production') {
    return drupal_get_path('module', 'salespush') . '/wsdl/' . variable_get('salespush_prod_wsdl');
  }
  return drupal_get_path('module', 'salespush') . '/wsdl/' . variable_get('salespush_sandbox_wsdl');
}

/**
 * Get username to SalesForce depending from the environment.
 *
 * @param string $environment
 *   Environment name.
 *
 * @return string
 *   String username.
 */
function salespush_get_username($environment) {
  if ($environment == 'production') {
    return variable_get('salespush_login');
  }
  return variable_get('salespush_sandbox_login');
}

/**
 * Get password to SalesForce api depending from the environment.
 *
 * @param string $environment
 *   Environment name.
 *
 * @return string
 *   Concatenated string.
 */
function salespush_get_password($environment) {
  if ($environment == 'production') {
    return variable_get('salespush_pass') . variable_get('salespush_token');
  }
  return variable_get('salespush_sandbox_pass') . variable_get('salespush_sandbox_token');
}
