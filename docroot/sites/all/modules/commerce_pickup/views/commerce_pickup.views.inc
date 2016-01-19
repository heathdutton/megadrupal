<?php

/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_pickup_views_data() {

  // Load the pickup location entity+bundle.
  $entity_bundle = variable_get('commerce_pickup_location_entity', NULL);

  // If an entity+bundle is not set, bail.
  if (empty($entity_bundle)) {
    return;
  }

  // Extract the entity type.
  $split = explode('__', $entity_bundle);
  $entity_type = $split[0];

  // Make sure the entity is valid.
  $entity = entity_get_info($entity_type);
  if (empty($entity)) {
    return;
  }

  // Create a new 'Commerce pickup' group.
  $data['commerce_pickup_line_item']['table']['group'] = t('Commerce Pickup');

  // Create implicit joins to the line item and pickup location entity tables.
  $data['commerce_pickup_line_item']['table']['join'] = array(
    'commerce_line_item' => array(
      'left_field' => 'line_item_id',
      'field' => 'line_item_id',
    ),
    $entity['base table'] => array(
      'left_field' => $entity['entity keys']['id'],
      'field' => 'entity_id',
    ),
  );

  // Line item entity.
  $data['commerce_pickup_line_item']['line_item_id'] = array(
    'title' => t('Line item'),
    'help' => t('Pickup location line item'),
    'relationship' => array(
      'base' => 'commerce_line_item',
      'base field' => 'line_item_id',
      'handler' => 'views_handler_relationship',
      'label' => t('Line item'),
    ),
  );

  // Pickup location entity.
  $data['commerce_pickup_line_item']['entity_id'] = array(
    'title' => t('Pickup location'),
    'help' => t('The pickup location entity.'),
    'relationship' => array(
      'base' => $entity['base table'],
      'base field' => $entity['entity keys']['id'],
      'handler' => 'views_handler_relationship',
      'label' => t('Pickup location'),
    ),
  );

  // Label field.
  $data['commerce_pickup_line_item']['label'] = array(
    'title' => t('Pickup location label'),
    'help' => t('The label of the pickup location entity.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}
