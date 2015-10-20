<?php
/**
 * @file
 *   Variable hooks implementations for the Cakemail API module.
 */

/**
 * Implements of hook_variable_info().
 */
function cakemail_api_variable_info($options) {
  return array(
    'cakemail_api_key' => array(
      'type' => 'string',
      'title' => t('CakeMail API key.', array(), $options),
      'default' => '',
      'localize' => FALSE,
    ),
    'cakemail_api_url' => array(
      'type' => 'string',
      'title' => t('Base URL for the CakeMail API REST endpoint.', array(), $options),
      'default' => CAKEMAIL_API_DEFAULT_ENDPOINT,
      'localize' => FALSE,
    ),
    'cakemail_cache_maxage' => array(
      'type' => 'number',
      'title' => t('Maximum age (in seconds) of cached CakeMail API results.'),
      'default' => CAKEMAIL_API_DEFAULT_CACHE_MAXAGE,
      'localize' => FALSE,
    ),
    'cakemail_api_ssl_verify_peer' => array(
      'type' => 'enable',
      'title' => t('Require verification of SSL certificate used when contacting the Cakemail API REST endpoint.'),
      'default' => FALSE,
      'localize' => FALSE,
    )
  );
}