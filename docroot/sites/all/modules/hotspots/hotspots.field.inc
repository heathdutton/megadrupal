<?php

/**
 * @file hotspots.field.inc
 */

/**
 * Implementation of hook_field_info().
 */
function hotspots_field_info() {
  return array(
    'hotspot' => array(
      'label' => 'Hotspot',
      'description' => t('Creates map and area HTML elements to create image map hotspots.'),
      'default_widget' => 'hotspot_default',
      'default_formatter' => 'hotspot_default',
      'instance_settings' => array('responsive' => true),
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function hotspots_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['responsive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Support responsive images'),
    '#default_value' => $settings['responsive'] ? $settings['responsive'] : NULL,
  );

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function hotspots_field_is_empty($item, $field) {
  return !isset($item['coords']) || $item['coords'] === '';
}

/**
 * Implements hook_field_validate().
 */
function hotspots_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {}

/**
 * Implements hook_field_insert().
 */
function hotspots_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $value) {
    $items[$delta]['attributes'] = serialize($value['attributes']);
  }
}

/**
 * Implements hook_field_update().
 */
function hotspots_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $value) {
    $items[$delta]['attributes'] = serialize($value['attributes']);
  }
}

/**
 * Implements hook_field_load().
 */
function hotspots_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {

      $items[$id][$delta]['attributes'] = unserialize($item['attributes']);
    }
  }
}

/**
 * Implements hook_field_widget_info().
 */
function hotspots_field_widget_info() {
  return array(
    'hotspot_default' => array(
      'label' => t('Default'),
      'field types' => array('hotspot'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function hotspots_field_widget_form(&$form, $form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element += array('#type' => 'fieldset');

  $associate_image_field_options = array();
  foreach (array_intersect_key(field_info_instances($element['#entity_type'], $element['#bundle']), hotspots_image_field_map()) as $image_instance) {
    $associate_image_field_options[$image_instance['field_name']] = $image_instance['label'];
  }

  $element['associate_image_field'] = array(
    '#type' => 'select',
    '#title' => t('Associated image field'),
    '#options' => $associate_image_field_options,
    '#default_value' => isset($items[$delta]['associate_image_field']) ? $items[$delta]['associate_image_field'] : NULL,
  );

  $element['associate_image_delta'] = array(
    '#type' => 'select',
    '#title' => t('Associated image field delta'),
    '#options' => drupal_map_assoc(range(0, 20)),
    '#default_value' => isset($items[$delta]['associate_image_delta']) ? $items[$delta]['associate_image_delta'] : NULL,
  );

  $element['coords'] = array(
    '#type' => 'textarea',
    '#resizable' => FALSE,
    '#rows' => 2,
    '#title' => t('Coordinates'),
    '#default_value' => isset($items[$delta]['coords']) ? $items[$delta]['coords'] : NULL,
  );

  $element['shape'] = array(
    '#type' => 'select',
    '#title' => t('Shape'),
    '#options' => array('default' => t('Default'), 'circle' => t('Circle'), 'poly' => t('Polygon'), 'rect' => t('Rectangle')),
    '#default_value' => isset($items[$delta]['shape']) ? $items[$delta]['shape'] : NULL,
  );

  $element['href'] = array(
    '#type' => 'textfield',
    '#maxlength' => 2048,
    '#title' => t('Href'),
    '#default_value' => isset($items[$delta]['href']) ? $items[$delta]['href'] : NULL,
  );

  $element['attributes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Attributes'),
  );

  $element['attributes']['class'] = array(
    '#type' => 'textfield',
    '#title' => 'Class',
    '#default_value' => isset($items[$delta]['attributes']['class']) ? $items[$delta]['attributes']['class'] : NULL,
  );

  $element['attributes']['rel'] = array(
    '#type' => 'textfield',
    '#title' => 'Rel',
    '#default_value' => isset($items[$delta]['attributes']['rel']) ? $items[$delta]['attributes']['rel'] : NULL,
  );

  $element['attributes']['alt'] = array(
    '#type' => 'textfield',
    '#title' => 'Alt',
    '#default_value' => isset($items[$delta]['attributes']['alt']) ? $items[$delta]['attributes']['alt'] : NULL,
  );

  $element['attributes']['target'] = array(
    '#type' => 'select',
    '#title' => 'target',
    '#options' => drupal_map_assoc(array('_self', '_blank', '_parent', '_top')),
    '#default_value' => isset($items[$delta]['attributes']['target']) ? $items[$delta]['attributes']['target'] : NULL,
  );

  return $element;
}

/**
 * Implements hook_field_widget_error().
 */
function hotspots_field_widget_error($element, $error, $form, &$form_state) {}

/**
 * Implementation of hook_field_formatter_info().
 */
function hotspots_field_formatter_info() {
  return array(
    'hotspot_default' => array(
      'label' => t('Hotspot element'),
      'description' => t('Injects image map coordinates into the HTML.'),
      'field types' => array('hotspot'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function hotspots_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $maps = array();

  // Group all hotspot values into the image fields they reference.
  foreach ($items as $delta => $item) {
    $maps[$item['associate_image_field']][$item['associate_image_delta']][] = $item;
  }

  // Iterate over each image field to produce one map element per image.
  foreach ($maps as $field_name => $map) {
    foreach ($map as $delta => $areas) {
      // Create a map from the $field-delta
      $id = hotspots_id($field_name, $delta);
      $map_element = array(
        '#hotspots_field_name' => $field_name,
        '#hotspots_field_delta' => $delta,
        '#hotspots_id' => $id,

        '#attributes' => array('name' => $id, 'id' => $id),
        '#type' => 'html_tag',
        '#tag' => 'map',
        '#value' => '',
      );

      foreach ($areas as $area) {
        $area['attributes']['coords'] = $area['coords'];
        $area['attributes']['shape'] = $area['shape'];
        $area['attributes']['href'] = $area['href'];

        $map_element['#value'] .= '<area ' . drupal_attributes(array_filter($area['attributes'])) . ' />';
      }

      $element[] = $map_element;
    }
  }

  return $element;
}
