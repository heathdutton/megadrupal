<?php

/**
 * @file
 * OSM Route admin configuration functions.
 */

/**
 * Returns the array of node types options.
 *
 * @return array
 *   an array of node types options
 */
function osm_route_get_options_node_types() {
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
 * @param bool $empty
 *   The field has an empty choice.
 *
 * @return array
 *   The array of options
 */
function osm_route_get_options_fields($type, $empty = FALSE) {
  $all_fields = field_info_field_map();
  $options = array();
  if ($empty) {
    $options[""] = "";
  }
  foreach ($all_fields as $k => $f) {
    if ($f['type'] == $type) {
      $options[$k] = $k;
    }
  }
  return $options;
}

/**
 * Menu callback: OSM Route Settings Form.
 */
function osm_route_admin_settings() {
  $form = array();

  $form[OSM_ROUTE_YOURNAVIGATION_URL] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get(OSM_ROUTE_YOURNAVIGATION_URL, 'http://www.yournavigation.org/api/1.0/gosmore.php'),
    '#size' => 100,
    '#maxlength' => 255,
    '#title' => t('YOURS service API base url'),
    '#required' => TRUE,
    '#description' => t('OpenMaps route is based on a YOURS service. By default available at http://www.yournavigation.org/api/1.0/gosmore.php.'),
  );

  $form['osm_route_node_type'] = array(
    '#type' => 'select',
    '#options' => osm_route_get_options_node_types(),
    '#default_value' => variable_get('osm_route_node_type', 'article'),
    '#title' => t('Content type with the itinerary'),
    '#required' => TRUE,
    '#description' => t('For this content type the module will retrieve georeferenced content from a field and it will build the path.'),
  );

  $form['osm_route_field_input'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('osm_route_field_input'),
    '#options' => osm_route_get_options_fields('entityreference'),
    '#title' => t('Georeferenced content field'),
    '#required' => TRUE,
    '#description' => t('The field where to retrieve the points of the path to build. It must be a entity reference of a georeferenced content.'),
  );

  $form['osm_route_field_input_geofield'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('osm_route_field_input_geofield'),
    '#options' => osm_route_get_options_fields('geofield'),
    '#title' => t('Coordinates field'),
    '#required' => TRUE,
    '#description' => t('The field with the coordinates of the actual point in the referenced entity. It must be of type geofield.'),
  );

  $form['osm_route_field_output'] = array(
    '#type' => 'select',
    '#options' => osm_route_get_options_fields('geofield'),
    '#default_value' => variable_get('osm_route_field_output'),
    '#title' => t('Name of the field where to save the calculated path'),
    '#required' => TRUE,
    '#description' => t('This field will be filled with the path while saving the node.'),
  );

  $form['osm_route_field_instructions'] = array(
    '#type' => 'select',
    '#options' => osm_route_get_options_fields('text_long', TRUE),
    '#default_value' => variable_get('osm_route_field_instructions'),
    '#title' => t('Name of the field where to save the driving instructions'),
    '#required' => FALSE,
    '#description' => t('This field will be filled with driving instructions. 
      Please leave it empty if you don\'t intend to automatically retrieve it.'),
  );

  $form['osm_route_near_threshold'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('osm_route_near_threshold', 5e-2),
    '#title' => t('Near coordinate threshold'),
    '#description' => t('Defines the threshold after which a point is considered far 
      in the coordinate system from a given point in a given direction (latitude or longitude).') . '<br />' .
    t('This value is used by the algorithm for accepting routes; 
      no route is accepted if is not considered near provided the given threshold.') . ' <br />' .
    t('For instance, if the threshold is 0.01, the point is (10,10) a path starting from (10, 10.2) is not accepted.') . '<br />' .
    t('0.05 is a good compromise, use a smaller value for more precision, a bigger value for faster computation.'),
    '#size' => 10,
    '#maxlength' => 15,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
  );

  $form['osm_route_tries_number'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('osm_route_tries_number', 5),
    '#size' => 10,
    '#maxlength' => 15,
    '#title' => t('Number of iterative tries'),
    '#description' => t('When the route cannot be calculated or is not found, 
       we can suppose that one of the two points has fallen out of the bounds of a street.') . ' <br />' .
    t('In this case, the module tries to calculate the path from anywhere near. 
       This setting determines the maximum number of random points that are attempted.'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  return system_settings_form($form);
}
