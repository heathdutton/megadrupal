<?php
/**
 * @file
 * Defines service endpoints provided by the DRPG module.
 */

/**
 * Implements hook_default_services_endpoint().
 */
function drpg_default_services_endpoint() {
  $endpoints = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'drpg_rest_server';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'drpg_rest';
  $endpoint->authentication = array();
  $endpoint->server_settings = array(
    'formatters' => array(
      'json' => TRUE,
      'bencode' => FALSE,
      'jsonp' => FALSE,
      'php' => FALSE,
      'xml' => FALSE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/vnd.php.serialized' => TRUE,
      'application/x-www-form-urlencoded' => TRUE,
      'multipart/form-data' => TRUE,
      'text/xml' => TRUE,
      'application/xml' => FALSE,
    ),
  );
  $endpoint->resources = array(
    'user' => array(
      'actions' => array(
        'login' => array(
          'enabled' => '1',
          'settings' => array(
            'services' => array(
              'resource_api_version' => '1.0',
            ),
          ),
        ),
        'logout' => array(
          'enabled' => '1',
          'settings' => array(
            'services' => array(
              'resource_api_version' => '1.0',
            ),
          ),
        ),
      ),
    ),
  );
  $endpoint->debug = 1;

  $endpoints[] = $endpoint;

  return $endpoints;
}
