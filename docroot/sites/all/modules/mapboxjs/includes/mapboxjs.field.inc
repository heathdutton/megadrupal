<?php

/**
 * @file
 * Field hooks.
 */

/**
 * Implements hook_field_info().
 */
function mapboxjs_field_info() {
  return array(
    'mapboxjs_preset' => array(
      'label' => t('MapBox.js map preset'),
      'description' => t('Enables placement of a MapBox.js map as a field on an entity.'),
      'settings' => array(),
      'instance_settings' => array(),
      'default_widget' => 'mapboxjs_preset_select',
      'default_formatter' => 'mapboxjs_preset_default',
      'no_ui' => FALSE
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function mapboxjs_field_instance_settings_form($field, $instance) {

}

/**
 * Implements hook_field_is_empty().
 */
function mapboxjs_field_is_empty($item, $field) {

}


/**
 * Implements hook_field_widget_info().
 */
function mapboxjs_field_widget_info() {
  return array(
    'mapboxjs_select' => array(
      'label' => t('MapBox.js map preset'),
      'field types' => array('mapboxjs_preset'),
      'settings' => array(),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function mapboxjs_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // @TODO - Test for field settings that do not provide default value.
  switch ($instance['widget']['type']) {
    case 'mapboxjs_select':
      $options = array('' => t('-- No map selected --'));
      foreach (mapboxjs_get_presets() as $preset) {
        $options[$preset->name] = $preset->label;
      }
      $element['value'] = array(
        '#title' => $instance['label'],
        '#description' => $instance['description'],
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
      );
    break;
  };
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function mapboxjs_field_formatter_info() {
  return array(
    'mapboxjs_preset_default' => array(
      'label' => t('Default'),
      'field types' => array('mapboxjs_preset'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function mapboxjs_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();
  // Iterate through field values and load mapboxjs map presets.
  foreach ($items as $delta => $item) {
    $preset = mapboxjs_get_presets($item['value']);
    if ($preset != FALSE) {
      $element[$delta] = mapboxjs_render_preset($preset);
    }
  }

  return $element;
}
