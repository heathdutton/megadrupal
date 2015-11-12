<?php

/**
 * @file
 * Hooks provided by Views Fast Field module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define additional views fast field formatters.
 *
 * @param $field_type
 *   A field type, e.g. 'text_long'.
 * @param $field_name
 *   A field name, e.g. 'body'.
 *
 * @return
 *   An array of additional formatters. Each formatter is an associative array
 *   that contains the following key-value pairs:
 *   - "title": The formatter title displayed in the field editing window.
 *   - "callback": The function to call to format the field item.
 */
function hook_views_fast_field_formatter($field_type, $field_name) {
  $formatters = array();

  switch ($field_type) {
    case 'geofield':
      $formatters[] = array(
        'title' => t('Digital co-ordinates as a single element'),
        'callback' => 'callback_views_fast_geo_coordinates',
      );
      break;
  }

  return $formatters;
}

/**
 * @addtogroup callbacks
 * @{
 */

/**
 * Formats Geofield views fast field.
 *
 * Callback for hook_views_fast_field_formatter().
 *
 * @param $item
 *   A field item.
 * @param $field_type
 *   A field type, e.g. 'text_long'.
 * @param $field_name
 *   A field name, e.g. 'body'.
 * @param $field_formatter
 *   A standard field formatter selected in the Views UI.
 * @param $field_formatter_settings
 *   A standard field formatter settings selected in the Views UI.
 *
 * @return
 *   An HTML string representation of the formatted field.
 */
function callback_views_fast_geo_coordinates($item, $field_type, $field_name, $field_formatter, $field_formatter_settings) {
  $result = '';

  if (isset($item['lat']) && isset($item['lon'])) {
    $result = t('Latitude: !latitude <br/>Longitude: !longitude', array(
      '!latitude' => number_format($item['lat'], 6, '.', ''),
      '!longitude' => number_format($item['lon'], 6, '.', ''),
    ));
  }

  return $result;
}

/**
 * @} End of "addtogroup callbacks".
 */
