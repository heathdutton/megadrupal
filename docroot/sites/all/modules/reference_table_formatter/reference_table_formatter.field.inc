<?php
/**
 * @file
 * Contains all field API hooks.
 */

/**
 * Implements hook_field_formatter_info().
 */
function reference_table_formatter_field_formatter_info() {
  return array(
    'reference_table_formatter' => array(
      'label' => t('Table of Fields'),
      'field types' => array_keys(reference_table_formatter_load_plugins()),
      'settings' => array(
        'fields' => array(),
        'show_header' => '1',
        'view_mode' => 'default',
        'hide_empty' => '0',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function reference_table_formatter_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $instance['display'][$view_mode]['settings'];
  $element = array();

  $plugin = reference_table_formatter_load_plugin($field['type'], $field, $instance, $settings);

  if ($display['type'] === 'reference_table_formatter') {
    $element['show_header'] = array(
      '#title' => t('Show Header'),
      '#type' => 'checkbox',
      '#default_value' => $settings['show_header'],
    );
    $element['fields'] = array(
      '#title' => t('Fields'),
      '#description' => t('The fields to display. All fields will be shown if left blank.'),
      '#type' => 'checkboxes',
      '#options' => $plugin->get_renderable_fields(),
      '#default_value' => $settings['fields'],
      '#required' => FALSE,
    );
    $element['view_mode'] = array(
      '#title' => t('View Mode'),
      '#description' => t('Fields will be rendered within their respective call according to the chosen view mode.'),
      '#type' => 'select',
      '#options' => $plugin->get_view_modes(),
      '#default_value' => $settings['view_mode'],
    );
    $element['hide_empty'] = array(
      '#title' => t('Hide Empty'),
      '#description' => t('Hide columns which have no data for any rows.'),
      '#type' => 'checkbox',
      '#default_value' => $settings['hide_empty'],
    );
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function reference_table_formatter_field_formatter_settings_summary($field, $instance, $view_mode) {
  $settings = $instance['display'][$view_mode]['settings'];
  $plugin = reference_table_formatter_load_plugin($field['type'], $field, $instance, $settings);
  return $plugin->get_summary();
}

/**
 * Implements hook_field_formatter_view().
 */
function reference_table_formatter_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  if ($display['type'] !== 'reference_table_formatter') {
    return;
  }

  $settings = $display['settings'];
  $plugin = reference_table_formatter_load_plugin($field['type'], $field, $instance, $settings);

  return $plugin->render_table($items);
}
