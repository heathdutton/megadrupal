<?php
/**
 * @file
 * Views integration for the commerce booking fields.
 */

/**
 * Implements hook_views_data_alter().
 *
 * Register a relationship handler for every entity that has a bundle with the
 * commerce_booking field on.
 */
function commerce_booking_field_views_data_alter(&$data) {
// Event entity types are those that have the commerce_booking field on them.
  $event_entity_types = array();

  // Target entity types are those that have the commerce_booking_event field
  // on them. By default these are ticket and commerce_order entities.
  $target_entity_types = array();

  // Get all field instances.
  $fields = field_info_field_map();
  foreach ($fields as $field_name => $info) {
    switch ($info['type']) {
      // Add to our event entity types.
      case 'commerce_booking':
        foreach (array_keys($info['bundles']) as $entity_type) {
          $event_entity_types[$entity_type] = $field_name;
        }
        break;

      // Add to our target entity types.
      case 'commerce_booking_event':
        foreach (array_keys($info['bundles']) as $entity_type) {
          $target_entity_types[$entity_type] = $field_name;
        }
        break;
    }
  }

  foreach ($event_entity_types as $event_entity_type => $booking_field_name) {
    $event_entity_info = entity_get_info($event_entity_type);

    $event_entity = $event_entity_info['label'];
    if ($event_entity == t('Node')) {
      $event_entity = t('Content');
    }

    foreach ($target_entity_types as $target_entity_type => $event_field_name) {
      $event_field = field_info_field($event_field_name);
      $target_entity_info = entity_get_info($target_entity_type);
      $target_entity = $target_entity_info['label'];
      if ($target_entity == t('Node')) {
        $target_entity = t('Content');
      }

      // Build reverse relationships (from event to target).
      $pseudo_field_name = $event_entity_type . '_' . $event_field_name . '_' . $target_entity_type;
      $replacements = array(
        '@event_entity' => $event_entity,
        '@target_entity' => $target_entity,
        '!field_name' => $event_field_name,
      );
      $data[$event_entity_info['base table']][$pseudo_field_name]['relationship'] = array(
        'handler' => 'views_handler_relationship_entity_reverse',
        'field_name' => $event_field_name,
        'field table' => _field_sql_storage_tablename($event_field),
        'field field' => $event_field_name . '_entity_id',
        'base' => $target_entity_info['base table'],
        'base field' => $target_entity_info['entity keys']['id'],
        'label' => t('@target_entity referencing @event_entity through !field_name', $replacements),
        'group' => t('Commerce Booking'),
        'title' => t('Referencing entity'),
        'help' => t('A bridge to the @target_entity entity that is referencing @event_entity via !field_name', $replacements),
        'join_extra' => array(
          array(
            'table' => _field_sql_storage_tablename($event_field),
            'field' => _field_sql_storage_columnname($event_field_name, 'entity_type'),
            'value' => $event_entity_type,
          ),
          array(
            'table' => _field_sql_storage_tablename($event_field),
            'field' => 'entity_type',
            'value' => $target_entity_type,
          ),
        ),
      );
    }
  }
}

/**
 * Implements hook_field_views_data().
 */
function commerce_booking_field_field_views_data($field) {
  $data = field_views_field_default_views_data($field);

  // Get possible event entity types. These are entities that have the
  // commerce_booking field on them.
  $event_entity_types = array();
  $fields = field_info_field_map();
  foreach ($fields as $field_name => $info) {
    switch ($info['type']) {
      // Add to our event entity types.
      case 'commerce_booking':
        foreach (array_keys($info['bundles']) as $entity_type) {
          $event_entity_types[$entity_type] = $field_name;
        }
        break;
    }
  }

  foreach ($data as $table_name => $table_data) {
    // @todo: Make this actually work for multiple end point types
    foreach ($event_entity_types as $entity_type => $event_field_name) {
      $entity_info = entity_get_info($entity_type);
      $entity = $entity_info['label'];
      if ($entity == t('Node')) {
        $entity = t('Content');
      }

      $field_name = $field['field_name'] . '_entity_id';
      $parameters = array('@entity' => $entity, '!field_name' => $field['field_name']);
      $data[$table_name][$field_name]['relationship'] = array(
        'handler' => 'views_handler_relationship',
        'base' => $entity_info['base table'],
        'base field' => $entity_info['entity keys']['id'],
        'label' => t('@entity event referenced from !field_name', $parameters),
        'group' => t('Commerce Booking'),
        'title' => t('Referenced Event Entity'),
        'help' => t('A bridge to the @entity event that is referenced via !field_name', $parameters),
        'extra' => array(
          array(
            'table' => $table_name,
            'field' => $field['field_name'] . '_entity_type',
            'value' => $entity_type,
          ),
        ),
      );

      // @todo: Remove this somehow.
      break;
    }
  }

  return $data;
}
