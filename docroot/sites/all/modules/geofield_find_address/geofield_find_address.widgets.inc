<?php

/**
 * @file
 * Provides field widget hooks for geofield module.
 */

/**
 * Implements hook_field_widget_info().
 */
function geofield_find_address_field_widget_info() {
  return array(
    'geofield_find_address_geocode' => array(
      'label' => t('Geocode from Postal Address'),
      'field types' => array('geofield'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function geofield_find_address_field_widget_form(&$form, &$form_state, $field, $instance,
                                            $langcode, $items, $delta, $base) {
  $element = $base;
  $widget = $instance['widget'];
  $element['input_format'] = array(
    '#type' => 'value',
    '#attributes' => array('class' => array('geofield_input_format')),
    '#value' => GEOFIELD_INPUT_AUTO_DISCOVER,
  );
  switch ($widget['type']) {
    case 'geofield_find_address_geocode':
      $latlon_value = array(
        'lat' => '',
        'lon' => '',
      );
      if (isset($items[$delta]['lat'])) {
        $latlon_value['lat'] = floatval($items[$delta]['lat']);
      }
      if (isset($items[$delta]['lon'])) {
        $latlon_value['lon'] = floatval($items[$delta]['lon']);
      }
      $element['geom'] = array(
        '#type' => 'geofield_find_address_latlon',
        '#title' => check_plain($instance['label']),
        '#description' => $instance['description'],
        '#default_value' => $latlon_value,
        '#required' => $instance['required'],
        '#delta' => $delta,
        '#address_field' => isset($widget['settings']['geofield_find_address_field']) ? $widget['settings']['geofield_find_address_field'] : '',
      );
      $element['input_format']['#value'] = GEOFIELD_INPUT_LAT_LON;
      if ($widget['settings']['geofield_find_address_field']) {
        $element['#attached']['js'][] = drupal_get_path('module', 'geofield_find_address') . '/js/geofield_find_address.js';
        $element['#attached']['js'][] = 'https://maps.googleapis.com/maps/api/js?v=3.exp&signed_in=true';
      }
      break;
  }
  return $element;
}

/**
 * Implements field_widget_settings_form().
 */
function geofield_find_address_field_widget_settings_form($this_field, $instance) {
  $settings = $instance['widget']['settings'];
  $entity_fields = field_info_instances($instance['entity_type'], $instance['bundle']);
  $valid_fields = array();
  $valid_fields[] = t('None');

  // Get a list of all valid fields that we both support
  // and are part of this entity.
  foreach ($entity_fields as $field) {
    if ($field['widget']['module'] == 'addressfield') {
      $valid_fields[$field['field_name']] = $entity_fields[$field['field_name']]['label'];
    }
  }

  if (count($valid_fields) == 1) {
    drupal_set_message(t('You need to add at least one Postal Address field.'), 'warning');
  }

  $form['geofield_find_address_field'] = array(
    '#type' => 'select',
    '#title' => t('Geocode from field'),
    '#default_value' => isset($settings['geofield_find_address_field']) ? $settings['geofield_find_address_field'] : 0,
    '#options' => $valid_fields,
    '#description' => t('Select which field you would like to geocode from.'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements hook_element_info().
 */
function geofield_find_address_element_info() {
  return array(
    'geofield_find_address_latlon' => array(
      '#input' => TRUE,
      '#process' => array('geofield_find_address_latlon_element_process'),
      '#theme' => array('geofield_find_address_latlon'),
    ),
  );
}

/**
 * Process function for geofield_find_address_latlon.
 */
function geofield_find_address_latlon_element_process($element, &$form_values) {
  $element['#tree'] = TRUE;
  $element['#input'] = TRUE;
  $element['lat'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#required' => (!empty($element['#required'])) ? $element['#required'] : FALSE,
    '#default_value' => (!empty($element['#default_value']['lat'])) ? $element['#default_value']['lat'] : '',
    '#attributes' => array(
      'class' => array('geofield-lat'),
      'id' => $element['#address_field'] . '-lat-' . $element['#delta'] . '-geom',
    ),
  );

  $element['lon'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#required' => (!empty($element['#required'])) ? $element['#required'] : FALSE,
    '#default_value' => (!empty($element['#default_value']['lon'])) ? $element['#default_value']['lon'] : '',
    '#attributes' => array(
      'class' => array('geofield-lon'),
      'id' => $element['#address_field'] . '-lon-' . $element['#delta'] . '-geom',
    ),
  );
  $address_field = $element['#address_field'];
  drupal_add_js(array('geofield_find_address' => array('address_field_name' => $address_field)), 'setting');
  // Set this to false always to prevent notices.
  $element['#required'] = FALSE;

  return $element;
}

/**
 * Theme function for geofield_find_address_latlon widget.
 */
function theme_geofield_find_address_latlon($vars) {
  return drupal_render($vars['element']['lat']) . drupal_render($vars['element']['lon']);
}
