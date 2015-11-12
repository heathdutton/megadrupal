<?php

/**
 * Init $node->sky_fields param.
 *
 * @param object $node
 */
function sky_field_init_sky_fields($entity_type, $entity) {
  $sky_fields = sky_field_db_sky_fields_select($entity_type, $entity);
  drupal_alter('sky_fields', $entity_type, $entity, $sky_fields);
  sky_field_set_sky_fields($entity, $sky_fields, FALSE);
}

/**
 * Set sky_fields node param.
 *
 * @param object $node
 * @return array of node fields.
 */
function sky_field_set_sky_fields($entity, $sky_fields, $set_changed_param = TRUE) {
  $entity->sky_fields = $sky_fields;
  if ($set_changed_param) {
    $entity->sky_fields_changed = TRUE;
  }
}

function sky_field_is_sky_fields_changed($entity) {
  if (!empty($entity->sky_fields_changed)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Get sky_fields param from node.
 *
 * @param object $node
 * @return array of node fields.
 */
function sky_field_get_sky_fields($entity_type, $entity) {
  // Init node sky_fields param if it doesn't exist yet.
  if (!sky_field_is_sky_fields_init($entity)) {
    sky_field_init_sky_fields($entity_type, $entity);
  }
  // Return array of node fields if there are any.
  if (!empty($entity->sky_fields)) {
    return $entity->sky_fields;
  }
  return FALSE;
}

function sky_field_is_sky_fields_init($entity) {
  if (isset($entity->sky_fields)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Check if entity type is configured to have sky fields.
 *
 * @param string $type
 * @return bool TRUE/FALSE
 */
function sky_field_is_sky_field_entity($type, $entity) {
  if (!sky_field_is_sky_field_entity_type($type)) {
    return FALSE;
  }
  if ($type != 'node') {
    return TRUE;
  }
  else if (!sky_field_is_sky_field_node_type($entity->type)) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Check if entity type is configured to have sky fields.
 *
 * @param string $type
 * @return bool TRUE/FALSE
 */
function sky_field_is_sky_field_entity_type($type) {
  $types = sky_field_get_active_entity();
  if (empty($types[$type])) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Check if node type is configured to have sky fields.
 *
 * @param string $type
 * @return bool TRUE/FALSE
 */
function sky_field_is_sky_field_node_type($type) {
  $types = variable_get('sky_field_node_types', array());
  if (empty($types[$type])) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Create a new field. Field array should contain all values you need but not field id.
 *
 * @param array $field
 * @return bool result 
 */
function sky_field_create_sky_field(&$field) {
  drupal_alter('sky_field_presave', $field);
  $result = sky_field_db_field_insert($field);
  if ($result) {
    module_invoke_all('sky_field_insert', $field);
  }
  return $result;
}

/**
 * Load node field.
 *
 * @param int $fid
 * @return array / bool
 *   array - field.
 *   FALSE - field doesn't exist. 
 */
function sky_field_load_sky_field($fid) {
  $values = array(
    'id' => $fid
  );
  $sky_field = sky_field_db_field_select($values);
  if ($sky_field) {
    module_invoke_all('sky_field_load', $sky_field);
    return array_pop($sky_field);
  }
  return FALSE;
}

/**
 * Update field based. Input param should have id field.
 *
 * @param array $field - node field.
 * @return updated field 
 */
function sky_field_update_sky_field(&$field) {
  // Allows to change the field before it's saved in database.
  drupal_alter('sky_field_update', $field);
  return sky_field_db_field_update($field);
}

/**
 * Delete node field.
 *
 * @param array $field
 * @return bool
 *   TRUE - success
 *   FALSE - failure 
 */
function sky_field_delete_sky_field($field) {
  module_invoke_all('sky_field_delete', $field);
  $values = array(
    'id' => $field['id'],
  );
  return sky_field_db_field_delete($values);
}

/**
 * Delete all node fields.
 *
 * @param object $node 
 */
function sky_field_delete_entity_sky_fields($entity_type, $entity) {
  $values = array(
    'entity_type' => $entity_type,
    'entity_id' => sky_field_get_entity_id($entity_type, $entity),
  );
  return sky_field_db_field_delete($values);
}

/**
 * Get settings from node field.
 *
 * @param object $sky_field
 * @return array $settings.
 */
function sky_field_get_sky_field_settings($sky_field) {
  $settings = isset($sky_field['settings']) ? $sky_field['settings'] : FALSE;
  return $settings;
}

function sky_field_get_entity_id($entity_type, $entity) {
  list($id) = entity_extract_ids($entity_type, $entity);
  return $id;
}

function sky_field_get_active_entity() {
  $active_entity = variable_get('sky_field_entities');
  return $active_entity;
}

function sky_field_get_entity_menu_base() {
  $items = array();
  $items['node'] = 'node';
  $items['user'] = 'user';
  $items['comment'] = 'comment';
  $items['taxonomy_vocabulary'] = 'admin/structure/taxonomy';
  $items['taxonomy_term'] = 'taxonomy/term';
  // Add base menus of custom entities to use sky field UI.
  drupal_alter('sky_field_entity_menu_base', $items);
  return $items;
}

function sky_field_get_menu_basic_path($entity_type) {
  $base = sky_field_get_entity_menu_base();
  return $base[$entity_type];
}