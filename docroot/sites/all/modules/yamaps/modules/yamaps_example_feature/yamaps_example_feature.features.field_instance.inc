<?php
/**
 * @file
 * Exports field instances.
 */

/**
 * Implements hook_field_default_field_instances().
 */
function yamaps_example_feature_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-yandex_maps_example-body'
  $field_instances['node-yandex_maps_example-body'] = array(
    'bundle' => 'yandex_maps_example',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-yandex_maps_example-field_yandex_maps_example_field'
  $field_instances['node-yandex_maps_example-field_yandex_maps_example_field'] = array(
    'bundle' => 'yandex_maps_example',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'yamaps',
        'settings' => array(
          'behaviors' => array(
            0 => 'clickZoom',
            1 => 'dblClickZoom',
            2 => 'drag',
            3 => 'scrollZoom',
            4 => 'ruler',
            5 => 'rightMouseButtonMagnifier',
          ),
          'controls' => TRUE,
          'height' => '400px',
          'traffic' => TRUE,
          'width' => '100%',
          'yamaps_display_options' => array(
            'display_type' => 'map',
            'open_button_text' => 'Open map',
          ),
        ),
        'type' => 'field_yamaps_formatter_dynamic',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_yandex_maps_example_field',
    'label' => 'Yandex Map',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'yamaps',
      'settings' => array(),
      'type' => 'yamaps_field',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Yandex Map');

  return $field_instances;
}
