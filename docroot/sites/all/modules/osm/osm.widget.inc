<?php

/**
 * @file
 * Widget settings.
 */

/**
 * Implements hook_field_info().
 */
function osm_field_info() {
  return array(
    'osm_code_reference' => array(
      'label'             => t('OSM Code'),
      'description'       => t('This field handle the OSM code.'),
      'default_widget'    => 'options_select',
      'default_formatter' => 'osm_default_formater',
      'settings'          => array(
        'allowed_values' => array(
          array(
            'vocabulary' => '',
            'parent'     => '0',
          ),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function osm_field_is_empty($item, $field) {
  return empty($item['value']);
}

/**
 * Implements hook_field_formatter_info().
 */
function osm_field_formatter_info() {
  return array(
    'osm_default_formater' => array(
      'label'       => t('Default'),
      'field types' => array('osm_code_reference'),
      'settings'    => array(
        'num_entries' => 4,
        'display'     => 1,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function osm_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  $num_entries = array(t('none'),1, 2, 3, 4, 5, 6, 7, 8, 8, 10);
  $element['num_entries'] = array(
    '#type'          => 'select',
    '#title'         => t('Number of entries to show'),
    '#default_value' => $settings['num_entries'],
    '#options'       => $num_entries,
  );

  $output_display = array(t('Programme and Events'), t('Programme'), t('Events'));
  $element['display'] = array(
    '#type'          => 'select',
    '#title'         => t('Display'),
    '#default_value' => $settings['display'],
    '#options'       => $output_display,
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function osm_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $output_display = array(t('Programme and Events'), t('Programme'), t('Events'));
  $summary = t('Displaying <em>@display</em> with @num_entries entries.', array(
    '@num_entries' => $settings['num_entries'],
    '@display'     => $output_display[$settings['display']],
  ));

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function osm_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];

  switch ($display['type']) {
    case 'osm_default_formater':
      $element[0] = array(
        '#theme'        => 'osm_default_formater',
        '#value'        => $items[0]['value'],
        '#num_entries'  => $settings['num_entries'],
        '#kind_display' => $settings['display'],
      );

      break;
  }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function osm_field_widget_info() {
  return array(
    'osm_widgets_suggested' => array(
      'label'       => t('Autocomplete for predefined suggestions'),
      'field types' => array('osm_code_reference'),
      'settings'    => array(
        'size'              => 60,
        'autocomplete_path' => 'osm/autocomplete',
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function osm_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $widget = $element;
  $widget['#delta'] = $delta;

  switch ($instance['widget']['type']) {
    case 'osm_widgets_suggested':
      $widget += array(
        '#type'              => 'textfield',
        '#default_value'     => !empty($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
        '#autocomplete_path' => $instance['widget']['settings']['autocomplete_path'],
        '#size'              => 60,
      );

      break;
  }

  $element['value'] = $widget;

  return $element;
}
