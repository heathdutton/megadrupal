<?php
/**
 * @file
 * shibboleth_drupalauth_rest_service.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function shibboleth_drupalauth_rest_service_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'shibboleth_drupalauth';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'auth';
  $endpoint->authentication = array();
  $endpoint->server_settings = array();
  $endpoint->resources = array(
    'user' => array(
      'actions' => array(
        'validate' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['shibboleth_drupalauth'] = $endpoint;

  return $export;
}
