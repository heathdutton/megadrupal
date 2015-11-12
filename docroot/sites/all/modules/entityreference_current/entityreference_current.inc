<?php
/**
 * @file
 * Common non-hook functions for Entity Reference Current module.
 */
/**
 * Wrapper function to get context (e.g. from Menu Object or OG-context).
 *
 * @param $entity_type
 *   The entity type the entity.
 * @param $entity
 *   The entity object that is being checked.
 * @param $field
 *   The field info array.
 * @param $instance
 *   The instance info array.
 * @param $validate
 *   Determine if access validation should be performed. Defaults to TRUE.
 *
 * @return
 *   Array of IDs a user may view.
 */
function entityreference_current_get_values($field, $instance, $validate = TRUE) {
  if (!$instance['settings']['behaviors']['current']['status']) {
    // Do nothing when current is disabled for this field.
    return;
  }

  $field_name = $field['field_name'];
  $target_type = $field['settings']['target_type'];

  $cache = &drupal_static(__FUNCTION__, array());
  $identifier = array(
    $instance['entity_type'],
    $instance['bundle'],
    $field_name,
    $validate,
  );

  $identifier = implode(':', $identifier);

  if (isset($cache[$identifier])) {
    return $cache[$identifier];
  }

  $cache[$identifier] = $ids = array();

  // Check if we have cached values.
  if (!$ids) {
    $ids = entityreference_current_get_values_from_cache($field, $instance);
  }

  // Check if there are values in the Menu Object.
  if (!$ids) {
    $entity = entityreference_current_get_current_entity($field, $instance);
    if ($entity) {
      $entity_ids = entity_extract_ids($target_type, $entity);
      $ids = array($entity_ids[0]);
    }
  }

  if (!$ids || !$validate) {
    // No IDs found, or no validation is needed.
    $cache[$identifier] = $ids;
    return $ids;
  }

  $handler = entityreference_get_selection_handler($field, $instance);
  if (!$ids = $handler->validateReferencableEntities($ids)) {
    $cache[$identifier] = FALSE;
    return;
  }

  // Check access to the provided entities.
  $target_type = $field['settings']['target_type'];
  entity_load($target_type, $ids);
  foreach ($ids as $delta => $id) {
    $entity = entity_load_single($target_type, $id);
    if (!$entity || !entity_access('view', $target_type, $entity)) {
      unset($ids[$delta]);
    }
  }

  $cache[$identifier] = $ids;
  return $ids;
}
/**
 * Get the values from the cached form.
 *
 * @param $field
 *   The field info array.
 * @param $instance
 *   The instance info array.
 *
 * @see
 *   entityreference_current_get_values()
 */
function entityreference_current_get_values_from_cache($field, $instance) {
  // Try to get the form out of cache.
  if (!$form_build_id = isset($_GET['form_build_id']) ? $_GET['form_build_id'] : isset($_POST['form_build_id']) ? $_POST['form_build_id'] : NULL) {
    return;
  }

  $field_name = $field['field_name'];

  $form_state = array();
  form_get_cache($form_build_id, $form_state);

  // If successful, get the value from the form_state.
  return isset($form_state['entityreference_current'][$instance['entity_type']][$instance['bundle']][$field_name]) ? $form_state['entityreference_current'][$instance['entity_type']][$instance['bundle']][$field_name] : FALSE;
}
/**
 * Get values for prepopulating fields via Object.
 *
 * @param $field
 *   The field info array.
 * @param $instance
 *   The instance info array.
 *
 * @see
 *   entityreference_current_get_values()
 */
function entityreference_current_get_current_entity($field, $instance) {
  $target_type = $field['settings']['target_type'];
  $entity = menu_get_object($target_type);
  if (empty($entity) && !empty($instance['settings']['behaviors']['current']['use_uid']) && $target_type == 'user') {
    // If target is user check to see if we can get uid from entity(only for nodes)
    $entity = menu_get_object();
    if (!empty($entity->uid)) {
      $entity = user_load($entity->uid);
    }
  }
  return $entity;
}