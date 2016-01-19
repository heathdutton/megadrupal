<?php
/**
 * Process variables for user-profile.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account
 *
 * @see user-profile.tpl.php
 */



function template_preprocess_intel_visitor_profile(&$variables) {
  //$visitor = $variables['elements']['#visitor'];

  // Helpful $user_profile variable for templates.
  foreach (element_children($variables['elements']) as $key) {
    $region = 'main';
    if (!empty($variables['elements'][$key]['#region'])) {
      $region = $variables['elements'][$key]['#region'];
    }
    if (!isset($variables[$region])) {
      $variables[$region] = array();
    }
    if ($region == 'header') {
       $variables[$key] = $variables['elements'][$key];
    }
    else {
      $variables[$region][$key] = $variables['elements'][$key];
    }
  }
  // Preprocess fields.
  //field_attach_preprocess('intel_visitor', $visitor, $variables['elements'], $variables);
}

/**
 * Process variables for user-profile-item.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $element
 *
 * @see user-profile-item.tpl.php
 */
function template_preprocess_intel_visitor_profile_item(&$variables) {
  $variables['title'] = $variables['element']['#title'];
  $variables['value'] = $variables['element']['#markup'];
  $variables['attributes'] = '';
  if (isset($variables['element']['#attributes'])) {
    $variables['attributes'] = drupal_attributes($variables['element']['#attributes']);
  }
}

function template_preprocess_intel_visitor_profile_block(&$variables) {
  if (isset($variables['element'])) {
    $variables['title'] = isset($variables['element']['#title']) ? $variables['element']['#title'] : '';
    $variables['markup'] = isset($variables['element']['#markup']) ? $variables['element']['#markup'] : '';
  }
  $variables['attributes'] = '';
  if (!isset($variables['element']['#attributes'])) {
    $variables['element']['#attributes'] = array();
  }
  if (!isset($variables['element']['#attributes']['class'])) {
    $variables['element']['#attributes']['class'] = array();
  }
  $variables['element']['#attributes']['class'][] = 'profile-block';
  
  if (isset($variables['element']['#attributes'])) {
    $variables['attributes'] = drupal_attributes($variables['element']['#attributes']);
  }
}

/**
 * Process variables for user-picture.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account: A user, node or comment object with 'name', 'uid' and 'picture'
 *   fields.
 *
 * @see user-picture.tpl.php
 */
function template_preprocess_intel_visitor_picture(&$variables) {
  $variables['picture'] = '';
  $entity = $variables['entity'];

  $filepath = variable_get('intel_visitor_default_image_path', '');
  if (empty($filepath)) {
    $filepath = variable_get('user_picture_default', '');
  }
  if (isset($entity->data['image']) && !empty($entity->data['image']['url'])) {
    $filepath = $entity->data['image']['url'];
  }

  if (!empty($filepath)) {
    $alt = t("@user's picture", array('@user' => $entity->name));
    // If the image does not have a valid Drupal scheme (for eg. HTTP),
    // don't load image styles.
    if (FALSE && module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
      $variables['picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt));
    }
    else {
      $variables['picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt));
    }
    // TODO set correct user_access permission
    if (!empty($entity->vid) && user_access('access user profiles')) {
      $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
      $variables['picture'] = l($variables['picture'], $entity->uri(), $attributes);
    }
  }
}

function intel_visitor_format_profile_item($title, $value, $attributes = '') {
  $item = array(
    '#theme' => 'intel_visitor_profile_item',
    '#title' => $title,
    '#markup' => $value,
  );
  if ($attributes) {
    $item['#attributes'] = $attributes;
  }
  return $item;
}

function intel_visitor_render_profile_items($items) {
  $output = '';
  foreach ($items as $item) {
    $output .= render($item);
  }
  return $output;
}

function template_preprocess_intel_visitor_social_links (&$variables) {
  $entity = $variables['entity'];
  $properties = $entity->data;
  $property_info = intel_get_visitor_property_info_all();
  $variables['markup'] = '';
  foreach ($property_info AS $prop_name => $info) {
    if (!isset($info['category']) || ($info['category'] != 'social') || empty($info['icon'])) {
      continue;
    }
    $a = explode('.', $prop_name);
    $scope = $a[0];
    $namespace = $a[1];
    if (empty($properties[$namespace]) || empty($properties[$namespace]['url'])) {
      continue;
    }
    $prop = $properties[$namespace];
    $alt = t('@title profile (source: @source)',
      array(
        '@title' => $info['title'],
        '@source' => !empty($prop['_source']) ? $prop['_source'] : t('(not set)'),
      )
    );
    $iv = array(
      'path' => $info['icon'],
      'alt' => $alt,
      'title' => $alt,
    );
    $img = theme('image', $iv);
    $options = array(
      'html' => true,
      'attributes' => array(
        'target' => $namespace,
      ),
    );
    
    $vars = array(
      'link' => l($img, $prop['url'], $options),
      'class' => drupal_clean_css_identifier($namespace),
    );
    
    $variables['markup'] .= theme('intel_visitor_social_link', $vars);
  }
}

function template_preprocess_intel_visitor_location(&$variables) {
  $variables['title'] = '';
  $variables['markup'] = '';
  $entity = $variables['entity'];
  $location = $entity->getVar('data', 'location');
  if (empty($location)) {
    return;
  }
  
  $variables['title'] = t('Location');
  $location['name'] = $location['city'] . ', ' . $location['region']  . ', ' . $location['country']; 
  $vars = array(
    'locations' => array($location),
  );
  $map = theme('intel_map', $vars);
  $map .= $entity->location('map');
  $variables['markup'] = $map;
}

function template_preprocess_intel_map(&$variables) {
  $locations = $variables['locations'];
  
  static $map_index;
  if (!isset($map_index)) {
    $map_index = 0;
    $options = array('type' => 'external', 'weight' => -1);
    drupal_add_js('https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false', $options);
  }

  $variables['map_index'] = $map_index++;
  $variables['map_options'] = array();
  $variables['map_options']['zoom'] = 4;
  $variables['map_options']['center'] = array(
    'lat' => 0, 
    'lon' => 0,
  );
  $variables['locations'] = array();

  foreach ($locations AS $loc) {
    if (!isset($loc['lat']) && isset($loc['latitude'])) {
      $loc['lat'] = $loc['latitude'];
    }
    if (!isset($loc['lon']) && isset($loc['longitude'])) {
      $loc['lon'] = $loc['longitude'];
    }
    $variables['locations'][] = array(
      'lat' => $loc['lat'],
      'lon' => $loc['lon'],
      'title' => $loc['name'],
    );
    //$vars['locations_json'] .= "[" . $loc['lat'] . ", " . $loc['lon'] . ", " . "'" . (isset($loc['name']) ? $loc['name'] : '') . "'],\n";
    $variables['map_options']['center']['lat'] = $loc['lat']; 
    $variables['map_options']['center']['lon'] = $loc['lon']; 
  }
}

function template_preprocess_intel_visitor_browser_environment(&$variables) {
  $variables['title'] = '';
  $variables['markup'] = '';
  $entity = $variables['entity'];
  $env = $entity->getVar('data', 'environment');
  if (empty($env)) {
    return;
  }
  $variables['title'] = t('Browser environment');
  $items = array();
  $items[] = intel_visitor_format_profile_item(t('Browser'), $env['browser'] . " v" . $env['browserVersion']);
  $items[] = intel_visitor_format_profile_item(t('OS'), $env['operatingSystem'] . " " . $env['operatingSystemVersion']);
  if (!empty($env['mobileDeviceInfo'])) {
    $items[] = intel_visitor_format_profile_item(t('Mobile device'), $env['mobileDeviceInfo']);
  }
  $items[] = intel_visitor_format_profile_item(t('Screen'), $env['screenResolution']);
  $items[] = intel_visitor_format_profile_item(t('Language'), $env['language']);

  $variables['markup'] = intel_visitor_render_profile_items($items);  
}

/**
 * Process variables for intel-visitor-browser-env.tpl.php.
 *
 * @see user-picture.tpl.php
 */
function template_preprocess_intel_visitor_bio(&$variables) {
  $variables['markup'] = $variables['entity']->getVar('data', 'bio', 'summary');
}

/**
 * Process variables for user-picture.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $account: A user, node or comment object with 'name', 'uid' and 'picture'
 *   fields.
 *
 * @see user-picture.tpl.php
 */
function template_preprocess_intel_visitor_browser_location(&$variables) {
  $variables['intel_visitor_browser_location'] = '';
  $entity = $variables['entity'];
}

function template_preprocess_intel_visitor_visits_table(&$variables) {
  $vdata = $variables['entity']->data;
  $variables['title'] = '';
  $variables['markup'] = '';
  // generate visits table
  if (!empty($vdata['analytics_visits'])) {
    $rows = array();
    uasort($vdata['analytics_visits'], function ($a, $b) {
        if (!isset($a['time']) || !isset($b['time'])) {
          return 1;
        }
        return ($a['time'] < $b['time']) ? 1 : -1;
      }
    );
    foreach ($vdata['analytics_visits'] AS $i => $visit) {
      if (substr($i, 0, 1) == '_') {
        continue;
      }
      $medium = isset($visit['trafficsource']['medium']) ? $visit['trafficsource']['medium'] : t('(not set)');
      $source = isset($visit['trafficsource']['source']) ? $visit['trafficsource']['source'] : t('(not set)');
      $rows[] = array(
        isset($visit['time']) ? format_date($visit['time'], 'medium') : t('(not set)'),
        $medium . ' / ' . $source,
        $visit['entrance']['pageviews'],
        number_format($visit['score'], 2),
        l(t('view'), $variables['entity']->uri . '/clickstream', array('query' => array('visit-count' => $i))),
      );
    }
    if (count($rows)) {
      $tvars = array();
      $tvars['rows'] = $rows;
      $tvars['header'] = array(
        t('Visit time'),
        t('Traffic source'),
        t('Pageviews'),
        t('Value'),
        t('Ops'),
      );
      $table = theme('table', $tvars);
      $variables['title'] = t('Recent site visits') . ((count($rows) > 9) ? ' (last 10)' : '');
      $variables['markup'] = $table;
    }
  }
}