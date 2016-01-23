<?php

/**
 * @file
 * Contains functions that are not hook implementations.
 *
 * @package context_per_og
 */

/**
 * Generates context object for a given organic group.
 *
 * @param object $entity
 *   An organic group entity.
 *
 * @param string $type
 *   The entity type.
 *
 * @return object
 *   The context to be created.
 */
function context_per_og_generate_context($entity, $type) {
  ctools_include('export');
  ctools_include('cleanstring');

  list($entity_id, $vid, $bundle) = entity_extract_ids($type, $entity);
  $human_name = entity_label($type, $entity);
  $machine_name = ctools_cleanstring(strtolower($human_name), $settings = array('separator' => '_', 'max length' => 220));

  // Verify that context with this name does not already exist.
  if (context_load($machine_name, TRUE)) {
    $suffix = 0;
    do {
      print_r($machine_name);
      $new_name = $machine_name . '_' . $suffix;
      $suffix++;
    } while (context_load($new_name));
    $machine_name = $new_name;
  }

  $context = context_per_og_default_context($human_name, $machine_name, $entity, $entity_id);

  return $context;
}

/**
 * Returns the default context to be created for each group.
 *
 * @param string $human_name
 *   The human readable name of the organic group.
 *
 * @param string $machine_name
 *   The machine name of the context.
 *
 * @param object $group
 *   The group entity.
 *
 * @param int $group_id
 *   The entity id of the group.
 */
function context_per_og_default_context($human_name, $machine_name, $group, $group_id) {
  $vars = array(
    '@group' => check_plain($human_name),
    '@context' => $machine_name,
  );

  $context = ctools_export_new_object('context');
  $context->name = $machine_name;
  $context->disabled = TRUE;
  $context->api_version = 3;
  $context->conditions = array(
    'context_og_condition_group_node' => array(
      'values' => array(
        $group_id => $group_id,
      ),
      'options' => array(
        'node_form' => 0,
      ),
    ),
  );
  $context->reactions = array();
  $context->description = substr(t('Context for Organic Group @group', $vars), 0, 255);
  $context->tag = 'context_per_og';
  $context->condition_mode = 0;

  drupal_alter('context_per_og', $context, $group);

  return $context;
}

/**
 * Saves a record associating an organic group with a context.
 *
 * @param object $context
 *   The context to associate.
 *
 * @param string $entity_type
 *   The entity type.
 *
 * @param object $group
 *   The group entity to associate.
 */
function context_per_og_save_group_context_record($context, $entity_type, $group) {
  list($group_eid, $vid, $bundle) = entity_extract_ids($entity_type, $group);
  $inserted = db_insert('context_per_og')
    ->fields(array(
      'context_name' => $context->name,
      'group_eid' => $group_eid,
      'entity_type' => $entity_type,
    ))
    ->execute();
}

/**
 * Gets the context associated with a given organic group.
 *
 * @param int $entity_id
 *   The entity id of the group.
 *
 * @return array
 *   The context_per_og record for the given group.
 */
function context_per_og_load_group_context_record($entity_id) {
  $query = db_select('context_per_og', 'cpo');
  $query->condition('group_eid', $entity_id)
    ->fields('cpo', array(
      'context_name',
      'group_eid',
      'entity_type',
    ));
  $context_record = $query->execute()->fetchObject();

  return $context_record;
}

/**
 * Gets the context associated with a given organic group.
 *
 * @param int $entity_id
 *   The entity id of the group.
 *
 * @return object
 *   The context for the given group.
 */
function context_per_og_load_group_context($entity_id) {
  $query = db_select('context_per_og', 'cpo');
  $query->condition('group_eid', $entity_id)
    ->fields('cpo', array('context_name'));
  $context_name = $query->execute()->fetchField();
  $context = context_load($context_name);

  return $context;
}

/**
 * Deletes a context and its context_per_og record.
 *
 * @param object $context
 *   The context to be deleted.
 *
 * @return bool
 *   Whether the context was deleted.
 */
function context_per_og_delete_context($context) {
  db_delete('context_per_og')
    ->condition('context_name', $context->name)
    ->execute();
  $deleted = context_delete($context);
  context_invalidate_cache();

  return $deleted;
}

/**
 * Deletes a context and its context_per_og record by context name.
 *
 * @param string $context_name
 *   The name of the context to be deleted.
 *
 * @return bool
 *   Whether the context was deleted.
 */
function context_per_og_delete_context_by_name($context_name) {
  $context = context_load($context_name);
  db_delete('context_per_og')
    ->condition('context_name', $context_name)
    ->execute();
  $deleted = context_delete($context);
  context_invalidate_cache();

  return $deleted;
}
