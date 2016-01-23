<?php
/**
 * @file
 * sharedcontent_server_feature.default_services_client_connection.inc
 */

/**
 * Implements hook_services_client_connection_default_connections().
 */
function sharedcontent_server_feature_services_client_connection_default_connections() {
  $export = array();

  $connection = new stdClass();
  $connection->disabled = FALSE; /* Edit this to true to make a default connection disabled initially */
  $connection->api_version = 1;
  $connection->name = 'sharedcontent';
  $connection->admin_title = 'Shared Content';
  $connection->admin_description = '';
  $connection->version = 3;
  $connection->endpoint = 'sc/sharedcontent';
  $connection->config = array(
    'auth' => array(
      'plugin' => 'ServicesClientConnectionSessionAuth',
      'config' => array(
        'username' => 'admin',
        'password' => 'XksSfjtGRc',
      ),
    ),
    'server' => array(
      'plugin' => 'ServicesClientConnectionRestServer',
      'config' => array(
        'request_formatter' => 'json',
        'response_parser' => 'json',
      ),
    ),
    'request' => array(
      'plugin' => 'ServicesClientConnectionCurlRequest',
      'config' => array(),
    ),
  );
  $connection->debug = 1;
  $export['sharedcontent'] = $connection;

  return $export;
}
