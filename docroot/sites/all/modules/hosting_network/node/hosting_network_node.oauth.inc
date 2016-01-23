<?php
/**
 * @file
 * hosting_network_node.oauth.inc
 */

/**
 * Implements hook_default_oauth_common_context().
 */
function hosting_network_node_default_oauth_common_context() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 1;
  $context->name = 'hosting_network_node_context';
  $context->title = 'hosting_network_node_context';
  $context->authorization_options = array(
    'access_token_lifetime' => NULL,
    'page_title' => NULL,
    'message' => NULL,
    'warning' => NULL,
    'deny_access_title' => NULL,
    'grant_access_title' => NULL,
    'disable_auth_level_selection' => NULL,
    'signature_methods' => array(
      0 => 'HMAC-SHA1',
    ),
    'default_authorization_levels' => array(),
  );
  $context->authorization_levels = array();
  $export['hosting_network_node_context'] = $context;

  return $export;
}
