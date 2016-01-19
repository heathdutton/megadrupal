<?php
/**
 * @file
 * The theme functions of the Weather.com.au module.
 */

/**
 * Preprocess variables for weathercomau-widget-block.tpl.php
 */
function template_preprocess_weathercomau_widget_block(&$variables) {
  // Retrieve block info.
  $block_id = $variables['block_id'];
  $block_settings = $variables['block_settings'];

  // Build the block cache id.
  $block_cid = 'weathercomau:block:' . $block_id . ':' . drupal_strtolower($block_settings['state']) . ':' . drupal_strtolower($block_settings['city']);

  // Retrieve the feed.
  $wca = new WCAFeed(array(
    'state' => $block_settings['state'],
    'city' => $block_settings['city'],
  ));
  $wca->getFeed(array('cid' => $block_cid));

  // Define some variables to be used in the template file.
  $variables['display_current_conditions'] = $variables['block_settings']['current'];
  $variables['current_conditions'] = theme('weathercomau_current_conditions', array(
    'title' => t('@city Current Conditions', array('@city' => $block_settings['city'])),
    'current_conditions' => $wca->getCurrentConditions(),
  ));
  $variables['display_forecast'] = ($variables['block_settings']['days'] > 0);
  $variables['forecast'] = theme('weathercomau_forecast', array(
    'title' => t('@city Forecast', array('@city' => $block_settings['city'])),
    'forecasts' => $wca->getForecasts($block_settings['days']),
  ));
  $variables['credits'] = l('Provided by weather.com.au', $wca->getLink(), array(
    'attributes' => array('target' => '_blank'),
  ));
}


/**
 * Preprocess variables for weathercomau-current-conditions.tpl.php
 */
function template_preprocess_weathercomau_current_conditions(&$variables) {
  $variables['wrapper_classes'] = 'weathercomau-current-conditions';
  $variables['title_classes'] = 'weathercomau-title';
  $variables['content_classes'] = 'weathercomau-content';
  $variables['current_conditions_classes'] = array();

  foreach ($variables['current_conditions'] as $id => &$current_condition) {
    // Create classes for each one of the current conditions.
    $variables['current_conditions_classes'][$id] = array();
    $variables['current_conditions_classes'][$id][] = 'weathercomau-current-condition';
    $variables['current_conditions_classes'][$id][] = drupal_html_class('weathercomau-current-condition-' . $id);

    // Convert classes to a nice string ready to be used in HTML.
    $variables['current_conditions_classes'][$id] = implode(' ', $variables['current_conditions_classes'][$id]);

    // Work out the correct format for the current condition value.
    switch ($id) {
      case 'temperature':
      case 'dewPoint':
        $current_condition_value = weathercomau_format_temperature($current_condition);
        break;

      case 'humidity':
        $current_condition_value = weathercomau_format_humidity($current_condition);
        break;

      case 'windSpeed':
      case 'windGusts':
        $current_condition_value = weathercomau_format_wind_speed($current_condition);
        break;

      case 'pressure':
        $current_condition_value = weathercomau_format_pressure($current_condition);
        break;

      case 'rain':
        $current_condition_value = weathercomau_format_rainfall($current_condition);
        break;

      default:
        $current_condition_value = $current_condition;
    }

    // Explode $id just before uppercase letter.
    $pieces = preg_split('/(?=[A-Z])/', $id);

    // Use the pieces to generate a nice title for the current condition.
    $current_condition_title = drupal_ucfirst(implode(' ', $pieces));

    // Convert the current condition to an object.
    $current_condition = (object) array(
      'title' => check_plain($current_condition_title),
      'value' => check_plain($current_condition_value),
    );
  }
}


/**
 * Preprocess variables for weathercomau-forecast.tpl.php
 */
function template_preprocess_weathercomau_forecast(&$variables) {
  $variables['wrapper_classes'] = 'weathercomau-forecasts';
  $variables['title_classes'] = 'weathercomau-title';
  $variables['content_classes'] = 'weathercomau-content';
  $variables['forecasts_classes'] = array();

  foreach ($variables['forecasts'] as $id => &$forecast) {
    // Create classes for each one of the days.
    $variables['forecasts_classes'][$id] = array();
    $variables['forecasts_classes'][$id][] = 'weathercomau-forecast';
    $variables['forecasts_classes'][$id][] = drupal_html_class('weathercomau-forecast-' . $forecast['day']);
    $variables['forecasts_classes'][$id][] = drupal_html_class('weathercomau-forecast-' . ($id + 1));

    // Create even more classes (first and lass) for each one of the days.
    if (($id == 0) && ($id != (count($variables['forecasts']) - 1))) {
      $variables['forecasts_classes'][$id][] = 'weathercomau-forecast-first';
    }
    if (($id != 0) && ($id == (count($variables['forecasts']) - 1))) {
      $variables['forecasts_classes'][$id][] = 'weathercomau-forecast-last';
    }

    // Convert classes to a nice string ready to be used in HTML.
    $variables['forecasts_classes'][$id] = implode(' ', $variables['forecasts_classes'][$id]);

    // Convert the forecast to an object.
    $forecast = theme('weathercomau_forecast_day', array(
      'forecast' => (object) array(
        'day' => check_plain($forecast['day']),
        'icon' => theme('image', array(
          'path' => $forecast['iconUri'],
          'alt' => $forecast['iconAlt'],
        )),
        'description' => check_plain($forecast['description']),
        'min' => check_plain($forecast['min']),
        'max' => check_plain($forecast['max']),
      )
    ));
  }
}


/**
 * Formats temperatures.
 *
 * @param int $temperature
 *   Temperature in degree celsius.
 * @param string $unit
 *   Unit to be returned.
 *
 * @return string
 *   Formatted representation in the desired unit.
 */
function weathercomau_format_temperature($temperature, $unit = '') {
  // Calculate the temperature in fahrenheit.
  $fahrenheit = (int) ($temperature * 9 / 5) + 32;

  // Format the temperature.
  if ($unit == 'celsius') {
    $result = t('!temperature °C', array('!temperature' => $temperature));
  }
  elseif ($unit == 'fahrenheit') {
    $result = t('!temperature °F', array('!temperature' => $fahrenheit));
  }
  elseif ($unit == 'celsiusfahrenheit') {
    $result = t('!temperature_c °C / !temperature_f °F',
      array(
        '!temperature_c' => $temperature,
        '!temperature_f' => $fahrenheit
      )
    );
  }
  elseif ($unit == 'fahrenheitcelsius') {
    $result = t('!temperature_f °F / !temperature_c °C',
      array(
        '!temperature_f' => $fahrenheit,
        '!temperature_c' => $temperature
      )
    );
  }
  else {
    // Defaults to site's temperature unit.
    $result = weathercomau_format_temperature($temperature, weathercomau_variable_get('temperature_unit'));
  }

  return $result;
}


/**
 * Formats humidity.
 *
 * @param int $humidity
 *   The humidity in percentage.
 *
 * @return string
 *   Formatted representation of the humidity.
 */
function weathercomau_format_humidity($humidity) {
  return $humidity . ' ' . weathercomau_variable_get('humidity_unit');
}


/**
 * Formats wind speed.
 *
 * @param int $wind_speed
 *   Wind speed in km/h.
 * @param string $unit
 *   Unit to be returned.
 *
 * @return string
 *   Formatted representation in the desired unit.
 */
function weathercomau_format_wind_speed($wind_speed, $unit = '') {
  if ($unit == 'kmh') {
    $result = t('!speed km/h', array('!speed' => $wind_speed));
  }
  elseif ($unit == 'mph') {
    // Convert into mph.
    $speed = round($wind_speed * 0.62137, 1);
    $result = t('!speed mph', array('!speed' => $speed));
  }
  elseif ($unit == 'knots') {
    // Convert into knots.
    $speed = round($wind_speed * 0.53996, 1);
    $result = t('!speed knots', array('!speed' => $speed));
  }
  elseif ($unit == 'mps') {
    // Convert into meter per second.
    $speed = round($wind_speed * 0.27778, 1);
    $result = t('!speed meter/s', array('!speed' => $speed));
  }
  elseif ($unit == 'beaufort') {
    // Convert into Beaufort.
    $number = weathercomau_calculate_beaufort($wind_speed);
    $result = t('Beaufort !number', array('!number' => $number));
  }
  else {
    // Default to site's wind speed unit.
    $result = weathercomau_format_wind_speed($wind_speed, weathercomau_variable_get('windspeed_unit'));
  }

  return $result;
}


/**
 * Formats pressure.
 *
 * @param int $pressure
 *   Pressure in hPa.
 * @param string $unit
 *   Unit to be returned.
 *
 * @return string
 *   Formatted representation in the desired unit.
 */
function weathercomau_format_pressure($pressure, $unit  = '') {
  if ($unit == 'hpa') {
    $result = t('!pressure hPa', array('!pressure' => $pressure));
  }
  elseif ($unit == 'inhg') {
    $result = t('!pressure inHg', array('!pressure' => round($pressure * 0.02953, 2)));
  }
  elseif ($unit == 'mmhg') {
    $result = t('!pressure mmHg', array('!pressure' => round($pressure * 0.75006, 0)));
  }
  elseif ($unit == 'kpa') {
    $result = t('!pressure kPa', array('!pressure' => round($pressure / 10, 1)));
  }
  else {
    // Default to site's pressure unit.
    $result = weathercomau_format_pressure($pressure, weathercomau_variable_get('pressure_unit'));
  }

  return $result;
}


/**
 * Formats rainfall.
 *
 * @param int $rainfall
 *   The rainfall in millimetres.
 * @param string $unit
 *   Unit to be returned.
 *
 * @return string
 *   Formatted representation in the desired unit.
 */
function weathercomau_format_rainfall($rainfall, $unit = '') {
  if ($unit == 'mm') {
    $result = t('!rainfall mm', array('!rainfall' => $rainfall));
  }
  elseif ($unit == 'inches') {
    $result = t('!rainfall "', array('!rainfall' => round($rainfall / 25.4 * 100) / 100));
  }
  else {
    // Default to site's rainfall unit.
    $result = weathercomau_format_rainfall($rainfall, weathercomau_variable_get('rainfall_unit'));
  }

  return $result;
}


/**
 * Calculate Beaufort wind scale for given wind speed.
 *
 * @link http://en.wikipedia.org/wiki/Beaufort_scale
 *
 * @param int $wind_speed
 *   Wind speed in km/h.
 *
 * @return int
 *   Beaufort number.
 */
function weathercomau_calculate_beaufort($wind_speed) {
  $result = 0;

  if ($wind_speed >= 120) $result = 12;
  if ($wind_speed >= 103) $result = 11;
  if ($wind_speed >= 88) $result = 10;
  if ($wind_speed >= 76) $result = 9;
  if ($wind_speed >= 63) $result = 8;
  if ($wind_speed >= 51) $result = 7;
  if ($wind_speed >= 40) $result = 6;
  if ($wind_speed >= 30) $result = 5;
  if ($wind_speed >= 20) $result = 4;
  if ($wind_speed >= 12) $result = 3;
  if ($wind_speed >= 7) $result = 2;
  if ($wind_speed >= 1) $result = 1;

  return $result;
}
