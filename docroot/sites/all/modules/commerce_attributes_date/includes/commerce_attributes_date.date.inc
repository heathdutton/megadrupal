<?php

/**
 * @file
 * Define helper function for date module to provide attributes functionality.
 */

/**
 * Callback for returning allowed values for date field in product.
 *
 * @param array $field
 *   The field definition.
 *
 * @param stdclassobject $entity
 *   The entity object the field is attached to,
 *   or NULL if no entity exists (e.g. in field settings page).
 *
 * @return array
 *   An array of available dates in current content for field.
 */
function _commerce_attributes_date_date_field_allowed_values($field, $entity) {
  $product_reference = variable_get('commerce_attributes_date_product_reference_field_name', 'field_product');

  $product_query = db_select('node');

  $product_query->join('field_data_' . $product_reference, 'fdfp', 'fdfp.entity_id = node.nid');

  $product_query->condition('fdfp.' . $product_reference . '_product_id', $entity->product_id)
                ->fields('node', array('nid'));

  $nid = $product_query->execute()->fetchField();

  // If no content found, return from here.
  if (empty($nid)) {
    return array();
  }

  // If field_name is what we're looking for then return allowed list of arrays,
  // because we've hardcoded the bundle.
  if ($field['module'] == 'date') {
    $date_format = variable_get('commerce_attributes_date_date_format', 'medium');

    $columns = array_keys($field['columns']);
    foreach ($columns as $key => $column_name) {
      $columns[$key] = $field['field_name'] . '_' . $column_name;
    }
    $all_dates = array();

    // Select all values from table.
    $query = db_select('field_data_' . $field['field_name'], 'fdfcd');

    $query->join('commerce_product', 'cp', 'cp.product_id = fdfcd.entity_id');
    $query->join('field_data_' . $product_reference, 'fdfp', 'fdfp.' . $product_reference . '_product_id = cp.product_id');

    $query->condition('fdfcd.bundle', $entity->type)
          ->condition('fdfp.entity_id', $nid)
          ->fields('fdfcd', $columns);

    $all_dates_result = $query->execute();

    // Loop through all results and get the array of values to return.
    while ($date = $all_dates_result->fetchObject()) {
      // If date is not in UNIXTIMESTAMP format.
      if ($field['type'] != 'datestamp') {
        $value_timestamp = strtotime($date->{$columns[0]});
        $start_date = strtotime($date->{$columns[0]});
        if (!empty($columns[1])) {
          $start_date = $date->{$columns[0]};
          $date->{$columns[1]} = strtotime($date->{$columns[1]});
        }
      }
      else {
        $start_date = $date->{$columns[0]};
        $value_timestamp = $date->{$columns[0]};
      }

      $has_value2 = (!empty($columns[1]) ? (' - ' . format_date($date->{$columns[1]}, $date_format)) : '');
      $all_dates[$start_date] = format_date($value_timestamp, $date_format) . $has_value2;
    }

    // Once we've all values return an array.
    return $all_dates;
  }

  return array();
}
