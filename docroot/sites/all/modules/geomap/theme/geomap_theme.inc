<?php
/**
 * Theme functions and settings for the geomap module
 * @file geomap_theme.inc
 */

/**
 * Return the default key specified in the admin screen.
 * By creating a theme function, themers can override this on a per theme basis
 * Add PHP code to the theme function, and the key can be selected on a per domain basis too
 *
 * @ingroup themeable
 * @param $domain
 *   Optionally specify a domain
 * @return $key
 *   A valid Google Maps Key
 *
 */
function theme_geomap_google_key($variables) {
  $domain = $variables['domain'];
  if (!($domain)) {
    $domain = $_SERVER['HTTP_HOST'];
  }

  $query = db_select('geomap_googlemap_api_keys', 'ggak')
    ->fields('ggak', array('google_map_key'))
    ->condition('domain', $domain)
    ->range(0, 1);
  
  if ($key = $query->execute()->fetchField()) {
    return $key;
  }

  return NULL;
}

/**
 * Create a map div.
 *
 * The Javascript will use this div to render tha map.
 * This will remain empty if no geo microformats exist on the current page.
 *
 * @param $delta
 *   If more than one map needs to be displayed add an identifier
 * @ingroup themeable
 */
function template_preprocess_geomap(& $variables) {
  if (user_access('view geomap') && $google_key = theme('geomap_google_key')) {
    //TODO: Allow a different ID one day (use delta) 
    if (empty ($variables['delta']) ) {
      $variables['delta'] = 0;
    }
    
    $variables['mapid'] = 'map';
    $variables['classes'] = 'googlemap googlemap-' . $variables['delta'];
    $loading = t('loading');
    $variables['loader_image'] = theme('image', array('path' => drupal_get_path('module', 'geomap') . '/theme/ajax-loader.gif', 'alt' => $loading, 'title' => $loading));
    $GLOBALS['geomap'] = TRUE;
  } 
  elseif (user_access('administer site configuration')) {
    drupal_set_message(l(t('Enter your Google Map Key here.'), 'admin/config/content/geomap'), 'warning');
  }
}

/**
 * Theme the info window text
 *
 * @ingroup themeable
 * @param &$variables
 *   See geomap theme. $text (string), $classes (array), $id (string)
 *
 */
function template_preprocess_geomap_windowtext(& $variables) {
  if ($variables['text'] === NULL) {
    $variables = array();
    return; //there is nothing to do if there is no text
  }

  $variables['text_id'] = ($variables['text_id'] ? check_plain($variables['text_id']) : NULL); //no default ID to save on html size
  $variables['text_classes'] = (is_array($variables['text_classes']) && count($variables['text_classes']) ? "infowindow-text " . implode(' ', $variables['text_classes']) :  "infowindow-text");
  $variables['text'] = check_plain($variables['text']); //in order for images to be included, use check_markup instead of check_plain.
}

/**
 * Theme the info window text
 *
 * @ingroup themeable
 * @param &$variables
 *   See geomap_theme. $windowtext (array), $infowindow_classes (array), $infowindow_id (string)
 */
function template_preprocess_geomap_infowindow(& $variables) {
  if ($variables['windowtext'] === NULL) {
    unset($variables['infowindow_classes']);
    unset($variables['infowindow_id']);
    return; //there is nothing to do if there is no src
  }
  $variables['infowindow_id'] = ($variables['infowindow_id'] ? check_plain($variables['infowindow_id']) : NULL);
  $variables['infowindow_classes'] = (count($variables['infowindow_classes']) ? "infowindow " . implode(' ', $variables['infowindow_classes']) :  "infowindow");

  //Text
  $text = $variables['windowtext']['text'];
  $windowtext_classes = (empty($variables['windowtext']['windowtext_classes']) ? array() : $variables['windowtext']['windowtext_classes']);
  $windowtext_id = (empty($variables['windowtext']['windowtext_id']) ? array() : $variables['windowtext']['windowtext_id']);

  $variables['windowtext'] = theme('geomap_windowtext', array('text' => $text, 'text_classes' => $windowtext_classes, 'text_id' => $windowtext_id));
}

/**
 * Theme an individual marker based on icon and infowindow
 *
 * Uses template geomap-marker
 *
 * @ingroup themeable
 * @param &$variables
 *   See geomap theme. $src; $icon_classes (array); $icon_id (string); $options (array) => $name (string), $value (string), $classes (array);
 */
function template_preprocess_geomap_icon(& $variables) {
  if ($variables['src'] === NULL) {
    unset($variables['src']);
    unset($variables['icon_classes']);
    unset($variables['icon_id']);
    unset($variables['icon_options']);
    return; //there is nothing to do if there is no src
  }

  //Icon
  $variables['icon_id'] = ($variables['icon_id'] ? check_plain($variables['icon_id']) : NULL);
  $variables['icon_classes'] = (count($variables['icon_classes']) ? "icon " . implode(' ', $variables['icon_classes']) :  "icon");
  $variables['src'] = check_plain($variables['src']);

  //Options
  if (is_array($variables['icon_options'])) {
    $options = $variables['icon_options'];
    foreach ($options as $k => $option) {
      //drupal_set_message($option['option_name'] . ": " . $option['option_value']);
      if ($option['option_name'] && $option['option_value']) {
        $variables['icon_options'][$k]['option_classes'] = (($option['option_classes']) ? "option " . implode(' ', $option['option_classes']) :  "option");
        $variables['icon_options'][$k]['option_name'] = check_plain($option['option_name']);
        $variables['icon_options'][$k]['option_value'] = check_plain($option['option_value']); //TODO: Do we need to use something else than check_plain?
      }
      else {
        unset($variables['icon_options'][$k]);
      }
    }
  }
}

/**
 * Theme an individual marker based on icon and infowindow
 *
 * Uses template geomap-marker
 *
 * @ingroup themeable
 * @param &$variables
 *   See geomap theme. $icon (array), $infowindow (array)
 */
function template_preprocess_geomap_marker(& $variables) {
  $variables['marker_classes'] = "marker";
  $variables['marker_id'] = NULL; //as a default we do not require an id

  $icon = $variables['icon'];
  $infowindow = $variables['infowindow'];

  //if a custom icon has been sent, then render that, otherwise query the DB for a custom icon

  if (isset($icon)) {
    $variables['icon'] = theme('geomap_icon', array(
      'src' => (isset($icon['src']) ? $icon['src'] : NULL),
      'icon_classes' => (isset($icon['icon_classes']) ? $icon['icon_classes'] : NULL),
      'icon_id' => (isset($icon['icon_id']) ? $icon['icon_id'] : NULL),
      'icon_options' => (isset($icon['icon_options']) ? $icon['icon_options'] : NULL),
    ));
  }
  if (isset($infowindow)) {
    $variables['infowindow'] = theme('geomap_infowindow', array(
      'windowtext' => (isset($infowindow['windowtext']) ? $infowindow['windowtext'] : NULL),
    ));
  }
}

/**
 * Theme an individual geolocation based on the input
 *
 * Uses template geomap-geolocation
 *
 * @ingroup themeable
 * @param &$variables
 *   See geomap_theme. $title (string), $latitude (float), $longitude (float), $nid (String),
 *   $type (String), $path (String), $tids (array), $marker (array)
 *
 */
function template_preprocess_geomap_geolocation(& $variables) {
  //NODE:
  if (!empty($variables['nid']) && is_int($variables['nid']) && (int) $variables['nid'] > 0) {
    $variables['nid'] = (int) $variables['nid']; //make certain this is an integer
    $variables['title'] = check_plain($variables['title']); //the node display title may not contain HTML
    $variables['title_display'] = check_plain($variables['title_display']); //the node display title may not contain HTML
    $variables['type'] = check_plain($variables['type']); //NOTE: the node type must be a valid drupal type!!!
    //$variables['path'] = "" //TODO: how to check that the path is properly formatted
  }
  else {
    $variables['nid'] = NULL;
    //we do not care about setting the others to NULL. As long as NID is null, it will not be rendered.
  }

  //LAT & LNG COORDINATES
  $variables['latitude'] = (float) $variables['latitude'];
  $variables['longitude'] = (float) $variables['longitude'];

  if ($variables['latitude'] || $variables['longitude']) {
    $variables['latitude_display'] = (!empty($variables['latitude_display']) ? filter_xss($variables['latitude_display']) : NULL);
    $variables['longitude_display'] = (!empty($variables['longitude_display']) ? filter_xss($variables['longitude_display']) : NULL);
  }
  else {
    //here we assume that 0,0 is not a valid coordinate due to the fact that it is in the middle of the ocean.
    $variables['latitude'] = NULL;
    $variables['longitude'] = NULL;
  }

  //MARKER
  $marker_icon = $variables['marker']['icon'];
  $marker_infowindow = isset($variables['marker']['infowindow']) ? $variables['marker']['infowindow'] : NULL;

  $variables['marker'] = NULL;

  if ($marker_icon === NULL) {
    //try the database settings if a marker is set there
    $marker_icon = _geomap_load_icon($variables['type'], $variables['tids']);
  }

  if ($marker_icon || $marker_infowindow) {
    $variables['marker'] = theme('geomap_marker', array('icon' => $marker_icon, 'infowindow' => $marker_infowindow));
  }

  //CSS
  $variables['id'] = NULL; //defaults to null
  $variables['geolocation_classes'] = 'geo' . (isset($variables['microformat_visible']) && $variables['microformat_visible'] == 0 ? ' geo-hidden' : ''); //defaults to geo
}

/**
 * Theme all selected geolocations
 *
 * Uses template geomap-geolocations
 *
 * @ingroup themeable
 * @param &$variables
 *   See geomap_theme. $geolocations (array), $classes (array), $id (string)
 *
 */
function template_preprocess_geomap_geolocations(& $variables) {
  $variables['geolocations_classes'] = (count($variables['geolocations_classes']) ? "geolocations " . implode(' ', $variables['geolocations_classes']) : "geolocations");
  $variables['geolocations_id'] = ($variables['geolocations_id'] ? check_plain($variables['geolocations_id']) : NULL);

  //copy the array, we replace the $variables['geolocations'] array with the array of rendered objects
  $geolocations = $variables['geolocations'];
  $variables['geolocations'] = array();

  foreach ($geolocations as $geolocation) {
    $variables['geolocations'][] = theme('geomap_geolocation', $geolocation);
  }
}

/**
 * Helper function to load an icon setting from the database
 *
 * @param $type
 *   Node Type to load
 * @return array
 *   The array for the node type icon
 */
function _geomap_load_icon($node_type = 'page', $tids = array()) {
  //There has to be a more efficient way to load the icons. We are
  //querying once currently per node Taxonomy term and node type

  //Here the order needs to be remembered! The tids come ordered by vid weight and then by tid weight.
  //Terms are displayed on the website in order of lowest to highest. Therefore we will load the image
  //for the lowest first. If it is not present, we load the next.
  if (is_array($tids)) {
    while ($tid = array_shift($tids)) {
      $query = db_select('geomap_taxonomy_icon', 'gti')
        ->fields('gti')
        ->condition('tid', $tid)
        ->range(0, 1);
      if ($result = $query->execute()->fetchObject()) {
        $options = array();
        foreach ($result as $k => $value) {
          if (!($k == "icon" || $k == "tid")   && strlen($value)) {
            $options[] = array(
              'option_name' => $k,
              'option_value' => $value,
            );
          }
        }
        return array(
          'src' => (strlen($result->icon) ? $result->icon : NULL),
          'icon_options' => (count($options) ? $options : NULL),
        );
      }
    }
  }

  //no taxonomy specific image was found, load the node type defaults if there is one
  $query = db_select('geomap_nodetype_icon', 'gni')
    ->fields('gni')
    ->condition('type', $node_type)
    ->range(0, 1);
  if ($result = $query->execute()->fetchObject()) {
    $options = array();
    foreach ($result as $k => $value) {
      if (!($k == "icon" || $k == "type")   && strlen($value)) {
        $options[] = array(
          'option_name' => $k,
          'option_value' => $value,
        );
      }
    }
    return array(
      'src' => (strlen($result->icon) ? $result->icon : NULL),
      'icon_options' => (count($options) ? $options : NULL),
    );
  }

  //no taxonomy specific image was found, load the node type defaults if there is one
  $query = db_select('geomap_nodetype_icon', 'gni')
    ->fields('gni')
    ->condition('type', 'default')
    ->range(0, 1);
  if ($result = $query->execute()->fetchObject()) {
    $options = array();
    foreach ($result as $k => $value) {
      if (!($k == "icon" || $k == "type")   && strlen($value)) {
        $options[] = array(
          'option_name' => $k,
          'option_value' => $value,
        );
      }
    }
    return array(
      'src' => (strlen($result->icon) ? $result->icon : NULL),
      'icon_options' => (count($options) ? $options : NULL),
    );
  }

  //load the default
  return NULL;
}
