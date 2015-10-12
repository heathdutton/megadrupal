<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - field_collection plugin.
 */

$plugin = array(
  'title' => t('FC: Field Collection plugin'),
  'completeness check' => 'fc_field_collection_field_is_complete',
  'incomplete process' => 'fc_field_collection_incomplete_process',
  'can be empty' => TRUE,

  'field form' => array(
    'fc_check_entities' => array(
      '#type' => 'checkbox',
      '#title' => t('Check fields in field collections for completeness'),
      '#description' => t('If checked, field collections will also be checked for completeness.'),
    ),
  ),
);

function fc_field_collection_field_is_complete($items, $instance, $field, &$completed = array()) {
  $module = $field['module'];
  $check_entities = !empty($instance['settings']['fc']['fc_check_entities']);

  foreach ($items as $delta => $item) {
    if ($check_entities) {
      // Loads each field_collection to find out whether it is complete
      $field_collection = field_collection_item_revision_load($item['revision_id']);
      if (!is_object($field_collection)) {
        $completed[] = FALSE;
      }
      else {
        $completed[] = $field_collection->fc->complete;
      }
    }
    else {
      // Just do a superficial check of item
      $completed[$delta] = !module_invoke($module, 'field_is_empty', $item, $field);
    }
  }
}

function fc_field_collection_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $check_entities = !empty($instance['settings']['fc']['fc_check_entities']);
  $incomplete = new fcIncomplete($field['field_name'], $parent, 'field_collection_item');

  $completed = array();
  if ($check_entities) {
    foreach ($items as $delta => $item) {
      // Loads each field_collection to find out whether it is complete
      $field_collection = field_collection_item_revision_load($item['revision_id']);
      $sub = new fcIncomplete($instance['label'], $parent, 'field_collection_item', $field_collection);
      $sub->process();
      $incomplete[] = $sub;
    }
  }

  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}