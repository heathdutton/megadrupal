<?php

/**
 * @file
 * Yandex Maps functions used across all components of the module.
 */

/**
 * Validate pixels or percentage value.
 *
 * @param array $element
 *   Field form element.
 * @param array $form_state
 *   Field form state.
 */
function yamaps_field_validate_pixels_percentage(array $element, array &$form_state) {
  $value = $element['#value'];
  if (!preg_match('/^[1-9]{1}[0-9]*(px|%)$/', $value)) {
    form_error(
      $element,
      t('%name must be a positive integer value and has "%" or "px" at the end.',
        array('%name' => $element['#title']))
    );
  }
}

/**
 * Get geo data for string.
 *
 * @param string $geolocation_string
 *   Name of geographical object.
 *
 * @return array|bool
 *   Geocoding array.
 */
function yamaps_geocoding($geolocation_string) {
  if (!$geolocation_string) {
    return FALSE;
  }

  // Preparing geocoding string.
  $query = array(
    'format' => 'json',
    'geocode' => $geolocation_string,
    'results' => 1,
    'lang' => 'ru',
  );

  $geolocation_url = url(YAMAPS_GEOCODER_URL,
    array('query' => $query, 'absolute' => TRUE));
  $geolocation_request = drupal_http_request($geolocation_url);

  if (isset($geolocation_request['data'])) {
    $geolocation_data = json_decode($geolocation_request['data']);

    if ($geolocation_data->response->GeoObjectCollection->metaDataProperty->GeocoderResponseMetaData->found > 0) {
      $map_center = $geolocation_data->response->GeoObjectCollection->featureMember[0]->GeoObject->Point->pos;
      $bounded_by = array(
        array_map('yamaps_var_to_float', array_reverse(explode(' ', $geolocation_data->response->GeoObjectCollection->featureMember[0]->GeoObject->boundedBy->Envelope->lowerCorner))),
        array_map('yamaps_var_to_float', array_reverse(explode(' ', $geolocation_data->response->GeoObjectCollection->featureMember[0]->GeoObject->boundedBy->Envelope->upperCorner))),
      );
      return array(
        'map_center' => array_map('yamaps_var_to_float', array_reverse(explode(' ', $map_center))),
        'bounds' => $bounded_by,
      );
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Set variable to float.
 *
 * @param string $val
 *   Variable that need to be set to float.
 *
 * @return float
 *   Float value.
 */
function yamaps_var_to_float($val) {
  return (float) $val;
}

/**
 * Wraps each element in array with specified tag and implodes array.
 *
 * @param array $arr
 *   Item that need to be wrapped.
 * @param string $wrap_start
 *   Wrap start string.
 * @param string $wrap_end
 *   Wrap end string.
 *
 * @return string
 *   String with wrapped elements.
 */
function yamaps_wrap_items(array $arr, $wrap_start, $wrap_end) {
  $items = array();

  foreach ($arr as $item) {
    $items[] = $wrap_start . $item . $wrap_end;
  }

  return implode('', $items);
}

/**
 * Returns behaviors list.
 *
 * @return array
 *   Types of behaviours.
 */
function yamaps_get_behaviors_list() {
  return array(
    'clickZoom' => t('Click Zoom'),
    'scrollZoom' => t('Scroll Zoom'),
    'dblClickZoom' => t('Double click zoom'),
    'drag' => t('Click and drag'),
    'multiTouch' => t('Multi Touch support'),
    'ruler' => t('Ruler'),
    'rightMouseButtonMagnifier' => t('Right mouse button magnifier'),
  );
}

/**
 * Returns Map type list.
 *
 * @return array
 *   Types of maps.
 */
function yamaps_get_map_types() {
  return array(
    'yandex#map' => t('Schema'),
    'yandex#satellite' => t('Satellite'),
    'yandex#hybrid' => t('Hybrid'),
  );
}

/**
 * Prepares values for map.
 *
 * @param array $value
 *   Values of map.
 */
function yamaps_cleanup_values(array &$value) {
  $value['coords'] = isset($value['coords']) ? $value['coords'] : NULL;
  $value['type'] = isset($value['type']) ? $value['type'] : 'yandex#map';
  $value['placemarks'] = isset($value['placemarks']) ? $value['placemarks'] : NULL;
  $value['lines'] = isset($value['lines']) ? $value['lines'] : NULL;
  $value['polygons'] = isset($value['polygons']) ? $value['polygons'] : NULL;
  $value['routes'] = isset($value['routes']) ? $value['routes'] : NULL;
  $value['hide'] = isset($value['hide']) ? $value['hide'] : 0;
}

/**
 * Prepares values for js.
 *
 * @param array $clean_values
 *   Values of map.
 *
 * @return array
 *   Prepared values.
 */
function yamaps_format_values_to_js(array $clean_values) {
  return array(
    'coords' => (isset($clean_values['coords'])) ? drupal_json_decode($clean_values['coords']) : array(),
    'type' => $clean_values['type'],
    'placemarks' => drupal_json_decode($clean_values['placemarks']),
    'lines' => drupal_json_decode($clean_values['lines']),
    'polygons' => drupal_json_decode($clean_values['polygons']),
    'routes' => drupal_json_decode($clean_values['routes']),
  );
}
