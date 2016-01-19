<?php

/**
 * @file
 * Route Elevation admin configuration functions.
 */

define('OLR_NAME', 'Route Elevation');
define('OLR_MACHINE_NAME', 'route_elevation');

/**
 * Implements hook_help().
 */
function route_elevation_help($path, $arg) {
  switch ($path) {
    case 'admin/help#route_elevation':
      return '<p>'
              . t('Add elevation map and graph from a gpx file uploaded within the node.')
              . '<br />'
              . t('Please check the file Readme.txt inside the module folder for further information.')
              . '</p>';
  }
}

/**
 * Implements hook_menu().
 */
function route_elevation_menu() {
  $items = array();

  $items['admin/config/services/route_elevation'] = array(
    'title' => OLR_NAME,
    'description' => t('Settings for') . ' ' . OLR_NAME . '.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('route_elevation_admin_settings'),
    'access arguments' => array('administer site configuration'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

/**
 * Returns the array of node types options.
 *
 * @return array|node
 *   an array of node types options
 */
function route_elevation_get_options_node_types() {
  $types = node_type_get_types();
  $options = array();

  foreach ($types as $type) {
    $options[$type->type] = $type->name;
  }
  return $options;
}

/**
 * The options related to a field.
 *
 * @param string $type
 *   The type of the field.
 *
 * @return array
 *   The array of options
 */
function route_elevation_get_options_fields($type) {
  $allfields = field_info_field_map();
  $options = array();
  foreach ($allfields as $k => $f) {
    if ($f['type'] == $type) {
      $options[$k] = $k;
    }
  }
  return $options;
}

/**
 * Menu callback: Route Elevation Settings Form.
 */
function route_elevation_admin_settings() {
  $form = array();
  $form['#validate'][] = 'route_elevation_admin_validate';

  $form['route_elevation_field_gpx'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('route_elevation_field_gpx'),
    '#options' => route_elevation_get_options_fields('file'),
    '#multiple_toggle' => '1',
    '#title' => t('The field containing the GPX file'),
    '#required' => TRUE,
    '#description' => t('The field where to retrieve the points of the path.') .
    '<br />' . t('It must be a field of type file containing a GPX.'),
  );

  $form['route_elevation_show_map'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('route_elevation_show_map', 1),
    '#title' => t('Show the map with the route above the elevation profile'),
    '#required' => FALSE,
    '#description' => t('The elevation profile is usually provided with a Google Map
       showing the path retrieved by the GPX.') .
    '<br />' . t('You can turn off this map if not needed.'),
  );

  $form['route_elevation_map_zoom'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('route_elevation_map_zoom', 10),
    '#title' => t('Map zoom level'),
    '#required' => FALSE,
    '#description' => t('Select the map zoom level between 1 and 15. Default level is 10.'),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['route_elevation_samples_number'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('route_elevation_samples_number', 100),
    '#title' => t('Number of samples for the elevation profile'),
    '#required' => FALSE,
    '#description' => t('Select the number of samples to build the elevation profile. Google free APIs allow less than 512 samples.'),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  return system_settings_form($form);
}

/**
 * The callback function that validates form settings.
 *
 * Verifies whether the fields are correctly compiled.
 */
function route_elevation_admin_validate($form, &$form_state) {
  $value = $form_state['values']['route_elevation_map_zoom'];
  // If necessary add:
  // if (!is_numeric($value)) {
  // form_set_error('route_elevation_map_zoom',
  // t('Please enter an integer number.'));
  // return;
  // }
  $value = intval($value);
  if (($value < 1) || ($value > 15)) {
    form_set_error('route_elevation_map_zoom', t('Please a zoom level between [1-15].'));
  }
}
