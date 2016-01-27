<?php
/**
 * @file
 * services_sso_server_endpoint.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function services_sso_server_endpoint_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'basic_auth';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'auth';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'jsonp' => TRUE,
        'xml' => TRUE,
        'json' => TRUE,
        'php' => TRUE,
        'yaml' => TRUE,
        'bencode' => TRUE,
        'rss' => TRUE,
      ),
      'parsers' => array(
        'application/x-www-form-urlencoded' => TRUE,
        'application/x-yaml' => TRUE,
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'multipart/form-data' => TRUE,
      ),
    ),
  );
  $endpoint->resources = array(
    'node' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'relationships' => array(
        'files' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'system' => array(
      'actions' => array(
        'connect' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'login' => array(
          'enabled' => 1,
        ),
        'logout' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['basic_auth'] = $endpoint;

  $endpoint = new stdClass;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'profile_flag';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'profile-flag';
  $endpoint->authentication = array();
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'bencode' => TRUE,
        'json' => TRUE,
        'php' => TRUE,
        'rss' => TRUE,
        'xml' => TRUE,
        'yaml' => TRUE,
        'jsonp' => FALSE,
      ),
      'parsers' => array(
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'application/x-www-form-urlencoded' => TRUE,
        'application/x-yaml' => TRUE,
        'multipart/form-data' => TRUE,
      ),
    ),
  );
  $endpoint->resources = array(
    'taxonomy_vocabulary' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'getTree' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['profile_flag'] = $endpoint;

  return $export;
}
