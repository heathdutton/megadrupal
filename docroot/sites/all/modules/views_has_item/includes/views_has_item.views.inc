<?php

/**
 * @file
 * Provide views handler.
 */

/**
 * Implements hook_field_views_data_alter().
 *
 * We add a new data for each field.
 */
function views_has_item_field_views_data_alter(&$data, $field, $module) {
  // We copy the structure of the fields.
  $table_name = 'field_data_' . $field['field_name'];
  $table = &$data[$table_name];
  $field_name = $field['field_name'];
  $table[$field_name . '_has_item'] = array(
    'group' => $table[$field_name]['group'],
    'title' => $table[$field_name]['title'] . ' (' . $field_name . ':has_item)',
    'title short' => $table[$field_name]['title short'] . ':has_item',
    'help' => $table[$field_name]['help'] . ' ' . t('Show content if it has an item in this field'),
    'filter' => array(
      'table' => $table_name,
      'field_name' => $field_name,
      'handler' => 'views_has_item_handler_filter_boolean_operator',
      'label' => t('Has an item on @label', array('@label' => $table[$field_name]['title'])),
      'type' => 'yes-no',
    ),
  );
}
