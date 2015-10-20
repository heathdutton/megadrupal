<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - field_collection plugin.
 */

$plugin = array(
  'title' => t('FC: Entity Reference plugin'),
  'completeness check' => 'fc_entityreference_field_is_complete',
  'incomplete process' => 'fc_entityreference_incomplete_process',
  'can be empty' => TRUE,

  'field form' => array(
    'fc_check_entities' => array(
      '#type' => 'checkbox',
      '#title' => t('Check fields in referenced entities for completeness'),
      '#description' => t('If checked, any entities referenced by this field will also be checked for completeness.'),
    ),
  ),
);

function fc_entityreference_field_is_complete($items, $instance, $field, &$completed = array()) {
  $module = $field['module'];
  $check_entities = !empty($instance['settings']['fc']['fc_check_entities']);
  $entity_type = $field['settings']['target_type'];

  foreach ($items as $delta => $item) {
    if ($check_entities) {
      // Loads each entity to find out whether it is complete
      $entity = entity_load($entity_type, array($item['target_id']));
      $entity = reset($entity);
      $completed[] = $entity->fc->complete;
    }
    else {
      // Just do a superficial check of item
      $completed[$delta] = !module_invoke($module, 'field_is_empty', $item, $field);
    }
  }
}

function fc_entityreference_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $check_entities = !empty($instance['settings']['fc']['fc_check_entities']);
  $entity_type = $field['settings']['target_type'];
  $incomplete = new fcIncomplete($instance['field_name'], $parent, $entity_type);

  $completed = array();
  if ($check_entities) {
    foreach ($items as $item) {
      // Loads each entity to find out whether it is complete
      $entities = entity_load($entity_type, array($item['target_id']));
      $entity = reset($entities);
      $sub = new fcIncomplete('', $parent, $entity_type, $entity);
      $sub->process();
      $incomplete[] = $sub;
    }
  }

  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}
