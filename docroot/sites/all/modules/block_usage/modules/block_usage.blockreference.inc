<?php

/**
 * @file
 * Block Reference's shadow implementation of hook_block_usage().
 */

/**
 * Implements hook_block_usage().
 */
function blockreference_block_usage($module, $delta) {
  // Find block ids.
  $bids = _block_usage_bids($module, $delta);
  if (!$bids) {
    return array();
  }

  // Cycle through potential fields/sources to find field items.
  $fields = field_info_field_map();
  $entity_loads = $loads_by_field_name = array();
  foreach ($fields as $field_name => $field) {
    if ($field['type'] == 'blockreference') {
      $table_name = 'field_data_' . $field_name;
      $column_name = $field_name . '_bid';

      // Query this field table for above bids.
      $items = db_query("SELECT entity_type, entity_id FROM $table_name WHERE $column_name IN (:bids)", array(
        ':bids' => $bids,
      ));

      // Collect entity_type's and entity_id's associatively.
      foreach ($items as $item) {
        $entity_loads[ $item->entity_type ][] = $item->entity_id;
        $loads_by_field_name[ $item->entity_type ][ $item->entity_id ][] = $field_name;
      }
    }
  }

  // Load entities and stack 'em up into results.
  $matches = array();
  foreach ($entity_loads as $entity_type => $ids) {
    $entities = entity_load($entity_type, $ids);
    foreach ($entities as $id => $entity) {
      $uri = entity_uri($entity_type, $entity);
      $path = $uri['path'];
      $options = @$uri['options'] ?: array();
      $fields = $loads_by_field_name[ $entity_type ][ $id ];
      $matches[] = l($entity_type . ' # ' . $id, $path, $options) . ' (' . implode(', ', $fields) . ')';
    }
  }

  return $matches;
}
