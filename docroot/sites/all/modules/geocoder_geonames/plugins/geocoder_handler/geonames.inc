<?php
/**
 * @file
 * Plugin to provide a goenames geocoder.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Geonames Geocoder"),
  'description' => t('Geocodes via geonames geocoder'),
  'callback' => 'geocoder_geonames',
  'field_types' => array(
    'text',
    'text_long',
    'addressfield',
    'location',
    'text_with_summary',
    'computed',
    'taxonomy_term_reference',
  ),
  'field_callback' => 'geocoder_geonames_field',
  'settings_callback' => 'geocoder_geonames_form',
  'terms_of_service' => 'http://code.google.com/apis/maps/documentation/geocoding/#Limits',
);

/**
 * Process Markup.
 */
function geocoder_geonames($query, $options = array()) {
  geophp_load();
  $q = array(
    'name' => $query,
    'maxRows' => variable_get('geocoder_geonames_maxresults', '15'),
  );
  if (variable_get('geocoder_geonames_country', 'none') != 'none') {
    $q['country'] = variable_get('geocoder_geonames_country', 'none');
  }
  if (variable_get('geocoder_geonames_class', 'none') != 'none') {
    $q['featureClass'] = variable_get('geocoder_geonames_class', 'none');
  }
  $geometries = array();
  if ($results = geonames_query('search', $q)) {
    if (isset($results->results)) {
      foreach ($results->results as $result) {
        $geom = new Point($result['lat'], $result['lng']);

        // Add additional metadata to the geometry - it might be useful.
        $geom->data = array();
        $geom->data['geocoder_accuracy'] = $result['name'];
        $geom->data['geocoder_formatted_address'] = $result['name'];
        $geom->data['geocoder_address_components'] = $result['name'];

        $location['components'][t("Name")] = $result['name'];
        $country = geonames_countryinfo($result['countrycode']);
        $location['components'][t("Country")] = $country['name'];
        if (!empty($result['fcode'])) {
          $fcode = geonames_featureinfo($result['fcode']);
          $location['components'][t("Type")] = $fcode['name'];
        }

        $geom->data['geocoder_formatted_address'] .= ", " . $country['name'];

        $geometries[] = $geom;
      }
    }
  }
  // The connonical geometry is the first result.
  $geometry = array_shift($geometries);

  // If there are any other geometries, these are auxiliary geometries that
  // represent "alternatives".
  if (count($geometries)) {
    $geometry->data['geocoder_alternatives'] = $geometries;
  }

  return $geometry;
}
  /**
   * Process field.
   */
function geocoder_geonames_field($field, $field_item, $options = array()) {
  if ($field['type'] == 'text' || $field['type'] == 'text_long' || $field['type'] == 'text_with_summary' || $field['type'] == 'computed') {
    return geocoder_google($field_item['value'], $options);
  }
  if ($field['type'] == 'addressfield') {
    $address = geocoder_widget_parse_addressfield($field_item);
    return geocoder_google($address, $options);
  }
  if ($field['type'] == 'location') {
    $address = geocoder_widget_parse_locationfield($field_item);
    return geocoder_google($address, $options);
  }
  if ($field['type'] == 'taxonomy_term_reference') {
    $term = taxonomy_term_load($field_item['tid']);
    return geocoder_google($term->name);
  }
}

/**
 * Process Form.
 */
function geocoder_geonames_form($default_values = array()) {
  $form = array();

  $form['geometry_type'] = array(
    '#type' => 'select',
    '#title' => 'Geometry Type',
    '#options' => array(
      'point' => 'Point (default)',
      'bounds' => 'Bounding Box',
      'viewport' => 'Viewport',
    ),
    '#default_value' => isset($default_values['geometry_type']) ? $default_values['geometry_type'] : 'point',
  );

  $form['all_results'] = array(
    '#type' => 'checkbox',
    '#title' => 'Geocode all alternative results',
    '#default_value' => isset($default_values['all_results']) ? $default_values['all_results'] : FALSE,
    '#description' => 'Often an ambiguous address (such as "Springfield USA") can result in multiple hits. By default we only return the first (best guess) result. Check this to return all results as a Multi-Geometry (MultiPoint or MultiPolygon).',
  );

  $form['reject_results'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Reject Results',
    '#options' => array(
      'APPROXIMATE' => 'APPROXIMATE:  indicates that the returned result is approximate.',
      'GEOMETRIC_CENTER'   => 'GEOMETRIC_CENTER: indicates that the returned result is the geometric center of a result such as a polyline (for example, a street) or polygon (region).',
      'RANGE_INTERPOLATED' => 'RANGE_INTERPOLATED: indicates that the returned result reflects an approximation (usually on a road) interpolated between two precise points (such as intersections). Interpolated results are generally returned when rooftop geocodes are unavailable for a street address.',
      'ROOFTOP' => 'ROOFTOP: indicates that the returned result is a precise geocode for which we have location information accurate down to street address precision.',
    ),
    '#default_value' => isset($default_values['reject_results']) ? $default_values['reject_results'] : array(),
    '#description' => 'Reject results that do not meet a certain level of quality or precision. Check all types of results to reject.',
  );

  return $form;
}
