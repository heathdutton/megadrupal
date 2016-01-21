<?php
/**
 * @file
 * comstack.entity.inc
 */

/**
 * Implements hook_entity_info().
 */
function comstack_entity_info() {
  $entities = array();

  $entities['comstack_message_type'] = array(
    'label' => t('Message Type'),
    'controller class' => 'EntityAPIControllerExportable',
    'base table' => 'comstack_message_type',
    'fieldable' => TRUE,
    //'bundle of' => 'comstack_message',
    'exportable' => TRUE,
    'entity keys' => array(
      'id' => 'id',
      'name' => 'type',
      'label' => 'label',
    ),
    'module' => 'comstack',
    'admin ui' => array(
      'path' => 'admin/structure/comstack/types',
      // Weird load callback name thanks to hook_ENTITY_load, and the name
      // colliding with "comstack_message_type_load" from message_type_load.
      'menu wildcard' => '%_comstack_message_type',
    ),
    'access callback' => 'comstack_message_type_access_callback',
  );

  return $entities;
}

/**
 * Implements hook_entity_info_alter().
 */
function comstack_entity_info_alter(&$entity_info) {
  // Alter the message entity access callback to be our own access callback.
  $entity_info['message']['access callback'] = '_comstack_message_access';
}

/**
 * Implements hook_entity_property_info_alter().
 */
function comstack_entity_property_info_alter(&$info) {
  // Message type entity.
  $comstack_message_type_properties = &$info['comstack_message_type']['properties'];
  $comstack_message_type_properties = array();

  // Message entity.
  $message_properties = &$info['message']['properties'];

  // Updated gets to be a clone of timestamp, mostly.
  $message_properties['updated'] = $message_properties['timestamp'];
  $message_properties['updated']['label'] = t('Updated');
  $message_properties['updated']['description'] = t('The time the message was last updated.');
  $message_properties['updated']['schema field'] = 'updated';

  $message_properties['deleted'] = array(
    'label' => t('Deleted'),
    'description' => t('Whether the message has been deleted.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack settings',
    'schema field' => 'deleted',
    'type' => 'boolean',
  );
  $message_properties['viewed'] = array(
    'label' => t('Viewed'),
    'description' => t('Whether the message has been viewed, for simple use cases.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack settings',
    'schema field' => 'viewed',
    'type' => 'boolean',
  );
  $message_properties['weight'] = array(
    'label' => t('Weight'),
    'description' => t('Integer field used for sorting.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack settings',
    'schema field' => 'weight',
    'type' => 'integer',
  );
}

/**
 * Message type Entity functions.
 */

/**
 * Message type entity access callback.
 */
function comstack_message_type_access_callback($op, $entity = NULL, $account = NULL) {
  return user_access('administer comstack message_types', $account);
}

/**
 * Return a list of message types in an array, whole objects.
 */
function comstack_get_message_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('comstack_message_type', isset($type_name) ? array($type_name) : FALSE);

  return isset($type_name) ? reset($types) : $types;
}

/**
 * Function to return a flat array of message types. Will be used
 * in entity_metadata info and therefore views also.
 */
function comstack_get_message_types_flat() {
  $types = comstack_get_message_types();
  $flat = array();

  if ($types) {
    foreach ($types as $type) {
      $flat[$type->type] = check_plain($type->label);
    }
  }

  return $flat;
}

/* Entity CRUD functions. */

/**
 * Menu argument loader; Load a message type by string.
 * This function has been prefixed with an underscore to avoid collisions with
 * hook_ENTITY_TYPE_load and the entity 'message_type' provided by the message
 * module.
 *
 * @param $type
 *   The machine-readable name of a message type to load.
 * @return
 *   A message type array or FALSE if $type does not exist.
 */
function _comstack_message_type_load($type) {
  return comstack_get_message_types($type);
}

/**
 * Implements hook_entity_delete().
 */
function comstack_message_type_delete($entity) {
  // Delete an associated comstack message type if there is one.
  $comstack_message_type = _comstack_message_type_load($entity->name);

  if ($comstack_message_type) {
    entity_delete('comstack_message_type', $comstack_message_type->id);
  }
}

/**
 * Message entity access callback.
 */
function _comstack_message_access($op, $entity, $account = NULL, $entity_type = 'message') {
  /**
   * Account for Entity Metadata wrapper weirdness, it's possible through
   * \RestfulBase::checkPropertyAccess() that a message entity will be passed
   * in and to this function but without the damn entity. We provide enough
   * protection around content to just return TRUE in this situation, as what
   * else can you do? :/ message_access will just return TRUE anyway.
   */
  if (!$entity) {
    return TRUE;
  }

  // Try and find a callback based off of the bundle name.
  $func = '_comstack_message_access__' . $entity->type;
  if (function_exists($func)) {
    return $func($op, $entity, $account, $entity_type);
  }

  return message_access($op, $entity, $account, $entity_type);
}
