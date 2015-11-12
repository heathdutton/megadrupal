<?php

$plugin = array(
  'label' => t('Example widget'),
  'description' => t('My example widget'),
  'field types' => array('text'),
  // Your own callback function called by `hook_field_widget_form()`
  // Leaving this empty, the function name would default to `my_module_example_widget_form()`
  'form' => 'my_module_form_example_widget',
  // Your own callback function called by `hook_field_widget_settings_form()`
  // Leaving this empty, the function name would default to `my_module_example_widget_settings()`
  'settings form' => 'my_module_settings_example_widget',
  // ... plus any other values you want to pass to hook_field_widget_info().
);

/**
 * Callback function for building the widget form.
 *
 * @see hook_field_widget_form() for the parameters and what you need to do
 * inside.
 */
function my_module_example_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element += array(
    '#type' => 'textfield',
    '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
    '#size' => $instance['widget']['settings']['size'],
    '#maxlength' => $field['settings']['max_length'],
    '#attributes' => array('class' => array('text-full')),
  );
  return array('value' => $element);
}

/**
 * Callback function for building the widget settings form.
 *
 * @see hook_field_widget_settings_form() for the parameters and what you need
 * to do inside.
 */
function my_module_example_widget_settings($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  return array(
    'size' => array(
      '#type' => 'textfield',
      '#title' => t('Size of textfield'),
      '#default_value' => $settings['size'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    )
  );
}