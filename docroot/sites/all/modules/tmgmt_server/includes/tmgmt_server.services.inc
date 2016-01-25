<?php
/**
 * @file
 * tmgmt_server.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function tmgmt_server_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'translation_services';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'api/v1';
  $endpoint->authentication = array(
    'tmgmt_server' => 'tmgmt_server',
  );
  $endpoint->server_settings = array(
    'formatters' => array(
      'json' => TRUE,
      'bencode' => FALSE,
      'jsonp' => FALSE,
      'php' => FALSE,
      'rss' => FALSE,
      'xml' => FALSE,
    ),
    'parsers' => array(
      'application/x-www-form-urlencoded' => TRUE,
      'application/json' => FALSE,
      'application/vnd.php.serialized' => FALSE,
      'application/xml' => FALSE,
      'multipart/form-data' => FALSE,
      'text/xml' => FALSE,
    ),
  );
  $endpoint->resources = array(
    'remote-clients' => array(
      'operations' => array(
        'create' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'status' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'language-pairs' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'translation-job' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
        'create' => array(
          'enabled' => '1',
        ),
      ),
      'actions' => array(
        'cancel' => array(
          'enabled' => '1',
        ),
        'reject' => array(
          'enabled' => '1',
        ),
      ),
      'relationships' => array(
        'item' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 0;


  $export['translation_services'] = $endpoint;

  return $export;
}

