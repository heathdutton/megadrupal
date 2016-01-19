<?php
/**
 * @file
 *   Admin page callbacks and related function for the CakeMail API module.
 */

/**
 * Form build callback for CakeMail API settings.
 */
function cakemail_api_settings_page($form, &$form_state) {
  $form['cakemail_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#default_value' => variable_get('cakemail_api_key'),
  );
  $form['cakemail_cache_maxage'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum cache age'),
    '#description' => t('Some results from calls to the CakeMail are cached for performances. Enter the maximum age (in seconds) a single result cam be cached.'),
    '#default_value' => variable_get('cakemail_cache_maxage', CAKEMAIL_API_DEFAULT_CACHE_MAXAGE),
  );
  $form['cakemail_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('REST endpoint'),
    '#description' => t('The base URL of the REST endpoint.'),
    '#default_value' => variable_get('cakemail_api_url', CAKEMAIL_API_DEFAULT_ENDPOINT),
  );
  $form['cakemail_api_ssl_verify_peer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require verification of SSL certificate used when contacting the Cakemail API REST endpoint.'),
    '#default_value' => variable_get('cakemail_api_ssl_verify_peer'),
  );
  return system_settings_form($form);
}