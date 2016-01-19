<?php
/**
 * @file
 * The admin pages of the Weather.com.au module.
 */

/**
 * Page callback for admin/config/services/weathercomau.
 */
function weathercomau_config_form() {
  $form = array();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['settings']['weathercomau_num_widgets'] = array(
    '#type' => 'select',
    '#title' => t('Number of widgets'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
    '#default_value' => weathercomau_variable_get('num_widgets'),
    '#description' => t('The number of widgets that will be available as blocks.'),
    '#required' => TRUE,
  );

  $form['units'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display units'),
    '#description' => t('Specify which units should be used for displaying the weather data.'),
  );

  $form['units']['weathercomau_temperature_unit'] = array(
    '#type' => 'select',
    '#title' => t('Temperature'),
    '#default_value' => weathercomau_variable_get('temperature_unit'),
    '#options' => array(
      'celsius' => t('Celsius'),
      'fahrenheit' => t('Fahrenheit'),
      'celsiusfahrenheit' => t('Celsius / Fahrenheit'),
      'fahrenheitcelsius' => t('Fahrenheit / Celsius'),
    ),
    '#required' => TRUE,
  );

  $form['units']['weathercomau_windspeed_unit'] = array(
    '#type' => 'select',
    '#title' => t('Wind speed'),
    '#default_value' => weathercomau_variable_get('windspeed_unit'),
    '#options' => array(
      'kmh' => t('km/h'),
      'mph' => t('mph'),
      'knots' => t('Knots'),
      'mps' => t('meter/s'),
      'beaufort' => t('Beaufort'),
    ),
    '#required' => TRUE,
  );

  $form['units']['weathercomau_pressure_unit'] = array(
    '#type' => 'select',
    '#title' => t('Pressure'),
    '#default_value' => weathercomau_variable_get('pressure_unit'),
    '#options' => array(
      'hpa' => t('hPa'),
      'kpa' => t('kPa'),
      'inhg' => t('inHg'),
      'mmhg' => t('mmHg'),
    ),
    '#required' => TRUE,
  );

  $form['units']['weathercomau_rainfall_unit'] = array(
    '#type' => 'select',
    '#title' => t('Rainfall'),
    '#default_value' => weathercomau_variable_get('rainfall_unit'),
    '#options' => array(
      'mm' => t('Millimetres'),
      'inches' => t('Inches'),
    ),
    '#required' => TRUE,
  );

  $form['caching'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caching'),
  );

  $form['caching']['weathercomau_cache_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Cache lifetime'),
    '#description' => t('The <a href="!link">RSS Weather Feed Quota Policy</a> restricts the number of queries made per day. We recommend setting this cache option to at least 1 hour.', array(
      '!link' => url('http://www.weather.com.au/about/rss', array(
        'absolute' => TRUE,
      )),
    )),
    '#options' => weathercomau_cache_lifetime_options(),
    '#default_value' => weathercomau_variable_get('cache_lifetime'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
