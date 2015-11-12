<?php
/**
 * @file
 * showoff_feeds.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function showoff_feeds_field_default_field_instances() {
  $field_instances = array();
  
  // Exported field_instance: 'node-unit-field_feeds'
  $field_instances['node-unit-field_feeds'] = array(
    'bundle' => 'unit',
    'default_value' => NULL,
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'showoff_feeds',
        'settings' => array(),
        'type' => 'showoff_feeds_layout_format_default',
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
    'field_name' => 'field_feeds',
    'label' => 'Feeds',
    'required' => 1,
    'settings' => array(
      'allowed_views' => array(
        'feeds' => 'feeds',
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'showoff_feeds',
      'settings' => array(),
      'type' => 'showoff_feeds_layout_widget_default',
      'weight' => '-3',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Feeds');

  return $field_instances;
}
