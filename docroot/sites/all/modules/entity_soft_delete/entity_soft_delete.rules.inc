<?php

/**
 * Implements hook_rules_file_info().
 */
function entity_soft_delete_rules_file_info() {
  $items = array();
  $items[] = 'entity_soft_delete.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function entity_soft_delete_rules_condition_info() {
   $items = array();

   $items['entity_is_deleted'] = array(
    'group' => t("entity"),
    'label' => t("Content is Soft deleted"),
    'base' => 'entity_soft_delete_rules_condition_entity_is_deleted',
    'parameter' => array(
      'entity' => array('type' => 'entity', 'label' => t("Content")),
    ),
    'access callback' => 'rules_entity_integration_access',
  );

  $items['content_is_using_entity_soft_delete'] = array(
    'group' => t("entity"),
    'label' => t("Content is using Soft delete"),
    'base' => 'entity_soft_delete_rules_condition_content_is_using_entity_soft_delete',
    'parameter' => array(
      'entity' => array('type' => 'entity', 'label' => t("Content")),
    ),
    'access callback' => 'rules_entity_integration_access',
  );

  return $items;
}

/**
 * Implements hook_rules_event_info() on behalf of the entity module.
 */
function entity_soft_delete_rules_event_info() {
  $items = array();

  $items['entity_soft_delete_after_deleting'] = array(
    'label' => t("After soft deleting Content"),
    'group' => t("entity"),
    'variables' => rules_events_node_variables(t("Soft deleted content"), TRUE),
    'access callback' => 'rules_entity_integration_access',
  );

  $items['entity_soft_delete_after_undeleting'] = array(
    'label' => t("After Un-soft-deleting Content"),
    'group' => t("entity"),
    'variables' => rules_events_node_variables(t("Un-soft-delete content"), TRUE),
    'access callback' => 'rules_entity_integration_access',
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the entity module.
 */
function entity_soft_delete_rules_action_info() {
  $items = array();

  $items['entity_soft_delete_delete'] = array(
    'label' => t("Soft delete Content"),
    'group' => t("entity"),
    'base' => 'entity_soft_delete_delete_action',
    'parameter' => entity_soft_delete_rules_actions_parameters(t("Content to entity_soft_delete")),
    'access callback' => 'rules_entity_integration_access',
  );

  $items['entity_soft_delete_undelete'] = array(
    'label' => t("Un-entity_soft_delete Content"),
    'group' => t("entity"),
    'base' => 'entity_soft_delete_undelete_action',
    'parameter' => entity_soft_delete_rules_actions_parameters(t("Content to un-entity_soft_delete")),
    'access callback' => 'rules_entity_integration_access',
  );

  return $items;
}

/**
 * Returns the standard arguments for rule events.
 *
 * @param $entity_label
 *   A string representing the entity label
 * @param $update
 *   (optional) a boolean representing whether or not this is an update
 *
 * @return
 *   The standard arguments for rule events.
 */
function entity_soft_delete_rules_events_parameters($entity_label, $update = FALSE) {
  $arguments = array(
    'entity' => array('type' => 'entity', 'label' => $entity_label),
    'entity_type' => array('type' => 'string', 'label' => t("Entity type")),
    'author' => array('type' => 'user', 'label' => t("User who triggered this event")),
  );

  if ($update) {
    $arguments += array(
      'entity_unchanged' => array(
        'type' => 'entity',
        'label' => t("Unchanged Content"),
        'handler' => 'rules_events_entity_unchanged',
      ),
      'author_unchanged' => array('type' => 'user', 'label' => t("Owner of the unchanged content")),
    );
  }

  return $arguments;
}

/**
 * Returns the standard arguments for rule actions.
 *
 * @param $entity_label
 *   A string representing the entity label
 *
 * @return
 *   The standard arguments for rule actions.
 */
function entity_soft_delete_rules_actions_parameters($entity_label) {
  $items = array();

  $items['entity'] = array(
    'type' => 'entity',
    'label' => $entity_label,
  );

  $items['entity_type'] = array(
    'type' => 'string',
    'label' => $entity_label,
  );

  $items['author'] = array(
    'type' => 'user',
    'label' => t("User triggering this event"),
    'optional' => TRUE,
    'default value' => NULL,
  );

  return $items;
}

/**
 * Condition: Check if the entity is soft deleted
 *
 * @param $entity
 *   A entity object
 *
 * @return
 *   TRUE/FALSE depending on if the entity is deleted.
 */
function entity_soft_delete_rules_condition_entity_is_deleted($entity) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_object($entity)){
    cf_error_invalid_object($function_history, 'entity', WATCHDOG_WARNING);
    return FALSE;
  }

  if (property_exists($entity, 'deleted') && $entity->deleted !== FALSE){
    return TRUE;
  }

  return FALSE;
}

/**
 * Condition: Check if the entity has entity_soft_delete support enabled for its entity type
 *
 * @param $entity
 *   A entity object
 *
 * @return
 *   TRUE/FALSE depending on if the entity has entity_soft_delete support enabled for its entity type.
 */
function entity_soft_delete_rules_condition_content_is_using_entity_soft_delete($entity) {
  if (!is_object($entity)){
    $function_history = array();
    cf_error_invalid_object($function_history, 'entity', WATCHDOG_WARNING);
    return FALSE;
  }

  $entity_bundle_settings = entity_soft_delete_get_entity_bundle_settings(array('type' => $entity->type, 'bundle' => $entity->bundle));
  if ($entity_bundle_settings['enabled']){
      return TRUE;
  }

  return FALSE;
}

/**
 * Action: entity_soft_delete a entity
 *
 * $param $entity
 *   A entity object
 * @param $author
 *   (optional) A user object representing the user who is performing the delete operation
 *
 * @return
 *   An array containing the entity object stored in the key called 'entity'.
 */
function entity_soft_delete_delete_action($entity, $entity_type, $author = NULL) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_object($entity)){
    cf_error_invalid_object($function_history, 'entity');
    return array('entity' => $entity);
  }

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $entity_bundle_settings = entity_soft_delete_get_entity_bundle_settings(array('type' => $entity_type, 'bundle' => $bundle));
  if ($entity_bundle_settings['enabled']){
    watchdog('entity_soft_delete', "The rules entity_soft_delete action on entity %id could not be performed. Soft delete support is either disabled for this entity or there was an error loading the entity_soft_delete entity type settings for this entity.", array('%id' => $id));
    return array('entity' => $entity);
  }

  if (is_object($author)){
    $target_user = $author;
  }
  else {
    $target_user = cf_get_user(FALSE, $function_history);
  }

  if (entity_access('delete', $entity, $target_user)) {
    entity_soft_delete_operate_on_entities('fake_delete', array($entity), $entity_type, $target_user, $function_history);
  }

  return array('entity' => $entity);
}

/**
 * Action: Un-entity_soft_delete a entity
 *
 * $param $entity
 *   A entity object
 * @param $author
 *   (optional) A user object representing the user who is performing the delete operation
 *
 * @return
 *   An array containing the entity object stored in the key called 'entity'.
 */
function entity_soft_delete_undelete_action($entity, $entity_type, $author = NULL) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_object($entity)){
    cf_error_invalid_object($function_history, 'entity');
    return array('entity' => $entity);
  }

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $entity_bundle_settings = entity_soft_delete_get_entity_bundle_settings(array('type' => $entity_type, 'bundle' => $bundle));
  if ($entity_bundle_settings['enabled']){
    watchdog('entity_soft_delete', "The rules unentity_soft_delete action on entity %id could not be performed. Soft delete support is either disabled for this entity or there was an error loading the entity_soft_delete entity type settings for this entity.", array('%id' => $id));
    return array('entity' => $entity);
  }

  if (is_object($author)){
    $target_user = $author;
  }
  else {
    $target_user = cf_get_user(FALSE, $function_history);
  }

  if (entity_soft_delete_can_undelete($entity, $target_user, $function_history)) {
    entity_soft_delete_operate_on_entities('undelete', array($entity), $entity_type, $target_user, $function_history);
  }

  return array('entity' => $entity);
}
