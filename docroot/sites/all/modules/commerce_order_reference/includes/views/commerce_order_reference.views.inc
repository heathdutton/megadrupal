<?php

/**
 * Provide Order Reference related Views integration.
 */


/**
 * Implements hook_field_views_data().
 */
function commerce_order_reference_field_views_data($field) {
  $data = field_views_field_default_views_data($field);

  // Add a join to commerce_order so that it can be used by the inverse relationship.
  $data[_field_sql_storage_tablename($field)]['table']['join']['commerce_order'] = array(
    'left_field' => 'order_id',
    'field' => $field['field_name'] . '_order_id',
    'extra' => array(
      array('field' => 'deleted', 'value' => 0, 'numeric' => TRUE),
    ),
  );

  // Build an array of bundles the order reference field appears on.
  $bundles = array();

  foreach ($field['bundles'] as $entity => $bundles) {
    $bundles[] = $entity . ' (' . implode(', ', $bundles) . ')';
  }

  foreach ($data as $table_name => $table_data) {
    foreach ($table_data as $field_name => $field_data) {
      if (isset($field_data['filter']['field_name']) && $field_name != 'delta') {
        $data[$table_name][$field_name]['relationship'] = array(
          'title' => t('Referenced order'),
          'help' => t('Appears in: @bundles', array('@bundles' => implode(', ', $bundles))),
          'base' => 'commerce_order',
          'base field' => 'order_id',
          'handler' => 'views_handler_relationship',
          'label' => t('Order'),
        );
      }
    }
  }

  return $data;
}