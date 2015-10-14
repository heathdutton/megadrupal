<?php

/**
 * @file
 * achievements_embeddable.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function achievements_embeddable_default_services_endpoint() {
  $endpoints = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'achievements_embeddable';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'services/achievements';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'formatters' => array(
      'json' => TRUE,
      'jsonp' => TRUE,
      'bencode' => FALSE,
      'php' => FALSE,
      'rss' => FALSE,
      'xml' => FALSE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/x-www-form-urlencoded' => TRUE,
      'application/vnd.php.serialized' => FALSE,
      'application/xml' => FALSE,
      'multipart/form-data' => FALSE,
      'text/xml' => FALSE,
    ),
  );
  $endpoint->resources = array(
    'get_user_achievements' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'get_all_achievements' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'achievement_trigger' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 0;

  $endpoints['achievements_embeddable'] = $endpoint;

  return $endpoints;
}
