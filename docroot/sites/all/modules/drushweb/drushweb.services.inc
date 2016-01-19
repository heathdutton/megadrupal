<?php
/**
 * @file
 * drushweb.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function drushweb_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'drushweb';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'drushweb';
  $endpoint->authentication = array(
    'services_oauth' => array(
      'oauth_context' => 'drushweb',
      'authorization' => 'drushweb',
      'credentials' => 'consumer',
    ),
  );
  $endpoint->server_settings = array(
    'formatters' => array(
      'bencode' => TRUE,
      'json' => TRUE,
      'php' => TRUE,
      'xml' => TRUE,
      'jsonp' => FALSE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/vnd.php.serialized' => TRUE,
      'application/x-www-form-urlencoded' => TRUE,
      'application/xml' => TRUE,
      'multipart/form-data' => TRUE,
      'text/xml' => TRUE,
    ),
  );
  $endpoint->resources = array(
    'drush' => array(
      'actions' => array(
        'execute' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'drushweb',
            ),
          ),
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['drushweb'] = $endpoint;

  return $export;
}
