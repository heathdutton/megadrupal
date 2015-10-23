<?php
/**
 * @file
 * An API for working with Gmap3 Tools module.
 */

define('GMAP3_TOOLS_MAP_TYPE_ID_HYBRID', 'hybrid');
define('GMAP3_TOOLS_MAP_TYPE_ID_ROADMAP', 'roadmap');
define('GMAP3_TOOLS_MAP_TYPE_ID_SATELLITE', 'satellite');
define('GMAP3_TOOLS_MAP_TYPE_ID_TERRAIN', 'terrain');

define('GMAP3_TOOLS_CONTROL_POSITION_TOP_LEFT', 1);
define('GMAP3_TOOLS_CONTROL_POSITION_TOP', 2);
define('GMAP3_TOOLS_CONTROL_POSITION_TOP_CENTER', 2);
define('GMAP3_TOOLS_CONTROL_POSITION_TOP_RIGHT', 3);
define('GMAP3_TOOLS_CONTROL_POSITION_LEFT_CENTER', 4);
define('GMAP3_TOOLS_CONTROL_POSITION_LEFT', 5);
define('GMAP3_TOOLS_CONTROL_POSITION_LEFT_TOP', 5);
define('GMAP3_TOOLS_CONTROL_POSITION_LEFT_BOTTOM', 6);
define('GMAP3_TOOLS_CONTROL_POSITION_RIGHT', 7);
define('GMAP3_TOOLS_CONTROL_POSITION_RIGHT_TOP', 7);
define('GMAP3_TOOLS_CONTROL_POSITION_RIGHT_CENTER', 8);
define('GMAP3_TOOLS_CONTROL_POSITION_RIGHT_BOTTOM', 9);
define('GMAP3_TOOLS_CONTROL_POSITION_BOTTOM_LEFT', 10);
define('GMAP3_TOOLS_CONTROL_POSITION_BOTTOM', 11);
define('GMAP3_TOOLS_CONTROL_POSITION_BOTTOM_CENTER', 11);
define('GMAP3_TOOLS_CONTROL_POSITION_BOTTOM_RIGHT', 12);

define('GMAP3_TOOLS_ZOOM_CONTROL_STYLE_DEFAULT', 0);
define('GMAP3_TOOLS_ZOOM_CONTROL_STYLE_SMALL', 1);
define('GMAP3_TOOLS_ZOOM_CONTROL_STYLE_LARGE', 2);
define('GMAP3_TOOLS_ZOOM_CONTROL_STYLE_ANDROID', 4);

/**
 * Default markers position which does nothing.
 */
define('GMAP3_TOOLS_DEFAULT_MARKERS_POSITION', 'default');
/**
 * Default markers position which center markers on map.
 */
define('GMAP3_TOOLS_DEFAULT_MARKERS_POSITION_CENTER', 'center');
/**
 * Default markers position which center and zoom markers on map so they will
 * fit into map.
 */
define('GMAP3_TOOLS_DEFAULT_MARKERS_POSITION_CENTER_ZOOM', 'center zoom');

/**
 * Add and configure js which is needed for map rendering.
 *
 * @param array $options
 *   Gmap configuration array. Check $defaults array for list of all supported
 *   options.
 */
function gmap3_tools_add_map($options) {
  static $init = FALSE;
  if (!$init) {
    $init = TRUE;
    drupal_add_js(gmap3_tools_get_library_url(), 'external', array('scope' => 'footer'));
    drupal_add_js(drupal_get_path('module', 'gmap3_tools') . '/gmap3_tools.js', array('scope' => 'footer'));
  }

  // Merge custom map options with defaults.
  $options = gmap3_tools_get_options($options);

  // Add map configuration options to js.
  drupal_add_js(array('gmap3_tools' => array('maps' => array($options))), 'setting');
}

/**
 * Attaches the JS to a given form using '#attached'.
 *
 * @param array $options
 *   Gmap configuration array. Check $defaults array for list of all supported
 *   options.
 * @param array $form
 *   The form the JS gets attached to.
 */
function gmap3_tools_attach_js($options, array &$form) {
  // Add JS using the #attached property in order to prevent breaking on
  // validation errors.
  $form['#attached']['js'] = array(
    array(
      'data' => gmap3_tools_get_library_url(),
      'type' => 'external',
    ),
    array(
      'data' => drupal_get_path('module', 'gmap3_tools') . '/gmap3_tools.js',
      'scope' => 'footer',
    ),
  );

  // Merge custom map options with defaults.
  $options = gmap3_tools_get_options($options);

  // Add map configuration options to js.
  $form['#attached']['js'][] = array(
    'data' => array(
      'gmap3_tools' => array(
        'maps' => array($options)
      ),
    ),
    'type' => 'setting',
  );
}

/**
 * Returns the default JS settings for the map.
 *
 * @return array
 *   The default JS settings.
 */
function gmap3_tools_get_options($options) {
  static $defaults = array(
    // Map html element id.
    'mapId' => NULL,
    // Map options.
    'mapOptions' => array(
      // Center X of map.
      'centerX' => 0,
      // Center Y of map.
      'centerY' => 0,
      // Disable double click zooming.
      'disableDoubleClickZoom' => FALSE,
      // Show controls for map type change.
      'mapTypeControl' => TRUE,
      // Map type id.
      'mapTypeId' => GMAP3_TOOLS_MAP_TYPE_ID_ROADMAP,
      // The maximum zoom level which will be displayed on the map. If null,
      // then maximum zoom from the current map type is used instead.
      'maxZoom' => NULL,
      // The minimum zoom level which will be displayed on the map. If null,
      // then minimum zoom from the current map type is used instead.
      'minZoom' => NULL,
      // Show pan contro.
      'panControl' => FALSE,
      // Enable scroll wheel zooming.
      'scrollwheel' => TRUE,
      // Show street view controls.
      'streetViewControl' => FALSE,
      // Zoom of map.
      'zoom' => 4,
      // Enable zoom controls.
      'zoomControl' => TRUE,
      // Zoom controls options.
      'zoomControlOptions' => array(
        // Position of controls.
        'position' => GMAP3_TOOLS_CONTROL_POSITION_TOP_LEFT,
        // Controls style.
        'style' => GMAP3_TOOLS_ZOOM_CONTROL_STYLE_DEFAULT,
      ),
    ),
    // Each marker can have custom markerOptions array. You should be able to
    // use all properties from google.maps.MarkerOptions object specification.
    // See https://developers.google.com/maps/documentation/javascript/reference#MarkerOptions
    'markers' => array(),
    // Default markers options - will be applied to all markers in map.
    'markerOptions' => array(
      // Marker icon image URL.
      //'icon' => 'http://your-site/custom-marker-icon.png',
    ),
    // Info window options.
    'infoWindowOptions' => array(),
    // Custom options for gmap3_tools API.
    'gmap3ToolsOptions' => array(
      // Close current opened infoWindow when opening new one.
      'closeCurrentInfoWindow' => TRUE,
      // Default position and zoom level of markers on map.
      'defaultMarkersPosition' => GMAP3_TOOLS_DEFAULT_MARKERS_POSITION,
      // Try to position map center based on current user geo location.
      'geolocation' => FALSE,
    ),
  );

  return gmap3_tools_array_union_recursive($defaults, $options);
}

/**
 * Getter for the google maps JS library.
 *
 * @return string
 *   The library including API key, if one is set.
 */
function gmap3_tools_get_library_url() {
  // Visit https://code.google.com/apis/console and generate new api key if
  // you need it.
  $lib_url = '//maps.googleapis.com/maps/api/js?&sensor=false';
  if ($api_key = variable_get('gmap3_tools_api_key', NULL)) {
    $lib_url .= '&key=' . $api_key;
  }
  return $lib_url;
}

/**
 * Helper for creatin marker options array.
 *
 * @param float $lat
 * @param float $lng
 * @param string $title
 *   Title is used for marker label.
 * @param string $content
 *   Content is used for marker's info window popup.
 * @param array $marker_options
 *   Holds all marker options for this marker.
 *   @see https://developers.google.com/maps/documentation/javascript/reference#MarkerOptions
 *   for all posible options.
 *
 *   Special gmap3_tools options:
 *
 *     relative - boolean. If TRUE then lat and lng coordinates are applied from
 *       current map center.
 * 
 * @return array
 *   Marker options array.
 */
function gmap3_tools_create_marker($lat, $lng, $title = NULL, $content = NULL, $marker_options = array()) {
  $marker = array('lat' => $lat, 'lng' => $lng);
  if (isset($title)) {
    $marker['title'] = $title;
  }
  if (isset($content)) {
    $marker['content'] = $content;
  }

  if (!isset($marker_options['relative'])) {
    $marker_options['relative'] = FALSE;
  }
  
  $marker['markerOptions'] = $marker_options;
  
  return $marker;
}

/**
 * Recursive union of two arrays. Works similar to + array operator but is also
 * taking care case when array values are also arrays.
 *
 * @param array $array1
 *   First array for union operation.
 * @param array $array2
 *   Second array for union operation.
 * @return array.
 *   Union result.
 */
function gmap3_tools_array_union_recursive($array1, $array2) {
  $union = $array1;
  foreach ($array2 as $key => $value) {
    if (array_key_exists($key, $union) && is_array($union[$key]) && is_array($value)) {
      $union[$key] = gmap3_tools_array_union_recursive($union[$key], $value);
    }
    else {
      $union[$key] = $value;
    }
  }
  return $union;
}
