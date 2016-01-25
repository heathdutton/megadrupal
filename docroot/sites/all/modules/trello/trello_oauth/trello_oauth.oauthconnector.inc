<?php
/**
 * @file
 * trello_oauth.oauthconnector.inc
 */

/**
 * Implements hook_default_oauthconnector_provider().
 */
function trello_oauth_default_oauthconnector_provider() {
  $export = array();

  $provider = new stdClass();
  $provider->disabled = FALSE; /* Edit this to true to make a default provider disabled initially */
  $provider->api_version = 1;
  $provider->name = 'trello';
  $provider->title = 'Trello';
  $provider->url = 'https://api.trello.com';
  $provider->consumer_advanced = array(
    'oauth2' => 0,
    'signature method' => 'HMAC-SHA1',
    'authentication realm' => '',
    'request token endpoint' => '/1/OAuthGetRequestToken',
    'authorization scope' => '',
    'authorization endpoint' => '/1/OAuthAuthorizeToken',
    'access token endpoint' => '/1/OAuthGetAccessToken',
  );
  $provider->mapping = array(
    'fields' => array(
      'uid' => array(
        'resource' => 'https://api.trello.com/1/members/me',
        'method post' => 0,
        'field' => 'username',
        'querypath' => FALSE,
        'sync_with_field' => 'name',
      ),
      'name' => array(
        'resource' => 'https://api.trello.com/1/members/me',
        'method post' => 0,
        'field' => 'fullName',
        'querypath' => FALSE,
        'sync_with_field' => '',
      ),
    ),
    'format' => 'json',
  );
  $export['trello'] = $provider;

  return $export;
}
