<?php

/**
 * @file
 * Services defaut endpoint configuration functions for the Pushretail For Ubercart module.
 */

/**
 * Set defaut endpoint configuration.
 */
function uc_pushretail_default_services_endpoint() {
  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'pushretail';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'pushretail';
  $endpoint->authentication = array(
    'services' => 'services',
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
      'application/json' => TRUE,
      'application/vnd.php.serialized' => TRUE,
      'application/x-www-form-urlencoded' => TRUE,
      'application/xml' => TRUE,
      'multipart/form-data' => TRUE,
      'text/xml' => TRUE,
    ),
  );
  $endpoint->resources = array(
    'uc_pushretail_order' => array(
      'alias' => 'order',
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'uc_pushretail_dashboard' => array(
      'alias' => 'dashboard',
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'uc_pushretail_customers' => array(
      'alias' => 'customers',
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'uc_pushretail_products' => array(
      'alias' => 'products',
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'system' => array(
      'actions' => array(
        'connect' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'user' => array(
      'actions' => array(
        'login' => array(
          'enabled' => '1',
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
  $endpoint->debug = 0;
  
  $export['pushretail'] = $endpoint;

  return $export;
}
