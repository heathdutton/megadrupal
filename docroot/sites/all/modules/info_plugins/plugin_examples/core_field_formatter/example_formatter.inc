<?php

$plugin = array(
  'label' => t('My example field formatter'),
  // An array of field types the formatter supports
  'field types' => array('...'),
  // Your own callback function called by `hook_field_formatter_view()`
  // Leaving this empty, the function name would default to `my_module_example_formatter_view()`
  'view' => 'my_module_view_example_formatter',
  // Your own callback function called by `hook_field_formatter_settings_form()`
  // Leaving this empty, the function name would default to `my_module_example_formatter_settings_form()`
  'settings form' => 'my_module_settings_form_example_formatter',
  // Your own callback function called by `hook_field_formatter_settings_summary()`
  // Leaving this empty, the function name would default to `my_module_example_formatter_settings_summary()`
  'settings summary' => 'my_module_settings_summary_example_formatter'
  // ... plus any other values you want to pass to `hook_field_formatter_info()`
);

/**
 * Callback function for building the render array for the field value.
 *
 * @see hook_field_formatter_view() for the parameters and what you need to do
 * inside.
 */
function my_module_view_example_formatter($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  foreach ($items as $delta => $item) {
    $element[$delta] = array('#markup' => $item['value'] . ' - string appended.');
  }
  return $element;
}

/**
 * Callback function to specify the form elements for the formatter's settings.
 *
 * @see hook_field_formatter_settings_form() for the parameters and what you need
 * to do inside.
 */
function my_module_settings_form_example_formatter($field, $instance, $view_mode, $form, &$form_state) {
  $element = array();

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element['my_setting'] = array(
    '#title' => t('My setting'),
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => $settings['my_setting'],
  );

  return $element;
}

/**
 * Callback function to return a short summary for the formatter.
 *
 * @see hook_field_formatter_settings_summary() for the parameters and what you need
 * to do inside.
 */
function my_module_settings_summary_example_formatter($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  return $summary = t('My setting is @setting', array('@setting' => $settings['my_setting']));
}