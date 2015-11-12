<?php
// $Id;

/**
 * @file
 *   A Views backend to query the Last.fm API.
 */

/**
 * Display lastfm global settings form.
 *
 * @param $form_state
 *   FAPI form state
 * @return
 *   FAPI form definition
 * @ingroup forms
 * @see deploy_server_form_validate()
 * @see deploy_server_form_submit()
 */
function lastfm_settings_form() {
  $form = array();

  $form['lastfm_api_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => variable_get('lastfm_api_key', ''),
    '#description' => t('Your last.fm API key.') . l(t('Get one.'), 'http://www.last.fm/api/account'),
  );

  $form['lastfm_api_secret'] = array(
    '#title' => t('API secret'),
    '#type' => 'textfield',
    '#size' => 32,
    '#default_value' => variable_get('lastfm_api_secret', ''),
    '#description' => t('Your last.fm API secret.'),
  );

  $form['lastfm_api_url'] = array(
    '#title' => t('API URL'),
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => TRUE,
    '#default_value' => variable_get('lastfm_api_url', LASTFM_DEFAULT_API_URL),
    '#description' => t('Base URL to the last.fm API.'),
  );

  $form['lastfm_cache_time'] = array(
    '#title' => t('Cache time'),
    '#type' => 'textfield',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('lastfm_cache_time', LASTFM_DEFAULT_CACHE_TIME),
    '#description' => t('Number of minutes that results from the last.fm API should be cached.'),
  );

  return system_settings_form($form);
}
