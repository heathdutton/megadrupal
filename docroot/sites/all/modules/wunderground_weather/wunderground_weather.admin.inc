<?php

/**
 * @file
 * Wunderground admin settings.
 */

/**
 * Admin settings form.
 */
function wunderground_weather_admin_form() {

  // URL to get the api key.
  $wg_link = l(t('http://www.wunderground.com/weather/api'), 'http://www.wunderground.com/weather/api');

  $form = array();

  $form['wunderground_weather_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Wunderground API key'),
    '#description' => filter_xss(t('Get your API key at !url', array('!url' => $wg_link))),
    '#default_value' => variable_get('wunderground_weather_api_key', ''),
    '#required' => TRUE,
  );

  $form['wunderground_weather_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => _wunderground_weather_languages(),
    '#default_value' => variable_get('wunderground_weather_language', 'EN'),
  );

  $form['units'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unit of measurement'),
    '#collapsible' => FALSE,
  );

  $form['units']['wunderground_weather_degrees'] = array(
    '#type' => 'select',
    '#title' => t('Degrees'),
    '#options' => array('fahrenheit' => t('Fahrenheit'), 'celsius' => t('Celsius')),
    '#default_value' => variable_get('wunderground_weather_scale', 'celsius'),
  );

  $form['units']['wunderground_weather_distance'] = array(
    '#type' => 'select',
    '#title' => t('Distance'),
    '#options' => array('mi' => t('Miles'), 'km' => t('Kilometers')),
    '#default_value' => variable_get('wunderground_weather_distance', 'km'),
  );

  $form['units']['wunderground_weather_windspeed'] = array(
    '#type' => 'select',
    '#title' => t('Wind speed'),
    '#options' => array(
      'bft' => t('Beaufort'),
      'kph' => t('Kilometers per hour'),
      'mph' => t('Miles per hour'),
    ),
    '#default_value' => variable_get('wunderground_weather_windspeed', 'bft'),
  );

  $form['caching'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caching'),
    '#collapsible' => FALSE,
  );

  $form['caching']['wunderground_weather_cache'] = array(
    '#title' => t('Cache weather data'),
    '#description' => t('Caching will reduce the number of calls to Wunderground and is a lot faster. If you choose not to cache, you will have the most up to date data in your blocks.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('wunderground_weather_cache', 1),
  );

  $period = drupal_map_assoc(array(0, 60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400), 'format_interval');
  $form['caching']['wunderground_weather_cache_expire'] = array(
    '#type' => 'select',
    '#title' => t('Expiration of cached weather data'),
    '#default_value' => variable_get('wunderground_weather_cache_expire', 86400),
    '#options' => $period,
    '#description' => t('The maximum amount of time weather data should be cached.'),
  );


  $form['wunderground_weather_block_current_amount'] = array(
    '#title' => t('How many current weather condition blocks would you like?'),
    '#type' => 'textfield',
    '#default_value' => variable_get('wunderground_weather_block_current_amount', '1'),
    '#size' => 3,
    '#element_validate' => array('element_validate_number'),
  );

  $form['wunderground_weather_block_forecast_amount'] = array(
    '#title' => t('How many weather forecast blocks would you like?'),
    '#type' => 'textfield',
    '#default_value' => variable_get('wunderground_weather_block_forecast_amount', '1'),
    '#size' => 3,
    '#element_validate' => array('element_validate_number'),
  );

  return system_settings_form($form);
}
