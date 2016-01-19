<?php

/**
 * Implements hook_element_info().
 */
function geofield_ymap_element_info() {
  return array(
    'geofield_ymap' => array(
      '#input' => TRUE,
      '#process' => array('geofield_ymap_element_process'),
      '#theme' => array('geofield_ymap'),
      '#theme_wrappers' => array('form_element'),
      '#map_editable' => TRUE,
      '#map_object_types' => array('point'),
      '#map_objects' => '',
      '#map_center' => '',
      '#map_zoom' => '',
    ),
  );
}

/**
 * Element process callback.
 */
function geofield_ymap_element_process($element, &$form_state, $form) {
  $element['#tree'] = TRUE;

  if (isset($element['#value']['center'])) {
    $element['#map_center'] = $element['#value']['center'];
  }
  if (isset($element['#value']['zoom'])) {
    $element['#map_zoom'] = $element['#value']['zoom'];
  }

  $objects = $element['#map_objects'];
  if ($objects && is_array($objects)) {
    $objects = _geofield_ymap_json_encode($element['#map_objects']);
  }
  $element['objects'] = array(
    '#type' => 'hidden',
    '#default_value' => $objects,
  );
  unset($element['#map_objects']);

  $element['center'] = array(
    '#type' => 'hidden',
    '#default_value' => is_array($element['#map_center']) ? implode(',', $element['#map_center']) : $element['#map_center'],
  );

  $element['zoom'] = array(
    '#type' => 'hidden',
    '#default_value' => $element['#map_zoom'],
  );

  return $element;
}
