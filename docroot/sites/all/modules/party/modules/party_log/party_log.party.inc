<?php
/**
 * @file
 * Party General hook include.
 */

/**
 * Implements hook_party_data_set_attach().
 *
 * Store a message against the data set controller when an entity is attached.
 */
function party_log_party_data_set_attach($controller, $entity, $delta, $method, $reattach) {
  global $user;

  $entity_type = $controller->getDataInfo('entity type');
  $data_set_name = $controller->getDataInfo('name');

  $controller->party_log_messages[] = array(
    'message_type' => 'party_data_set_attach_' . $data_set_name,
    $entity_type => $entity,
    'timestamp' => time(),
    'account' => $user,
  );
}

/**
 * Implements hook_party_data_set_detach().
 *
 * Store a message against the data set controller when an entity is detached.
 */
function party_log_party_data_set_detach($controller, $entity, $delta) {
  global $user;

  $entity_type = $controller->getDataInfo('entity type');
  $data_set_name = $controller->getDataInfo('name');

  $controller->party_log_messages[] = array(
    'message_type' => 'party_data_set_detach_' . $data_set_name,
    $entity_type => $entity,
    'timestamp' => time(),
    'account' => $user,
  );
}

/**
 * Implements hook_party_data_set_save().
 *
 * Create and save any messages stored against the controller.
 */
function party_log_party_data_set_save($controller) {
  if (empty($controller->party_log_messages)) {
    return;
  }

  $entity_type = $controller->getDataInfo('entity type');
  $field_name = 'party_log_' . $entity_type;

  while ($log_message = array_shift($controller->party_log_messages)) {
    $type = $log_message['message_type'];

    // Make sure the entity is loaded
    $entity = $log_message[$entity_type];
    $info = entity_get_info($entity_type);
    $idKey = $info['entity keys']['id'];
    $entity = entity_load_single($entity_type, $entity->$idKey);

    // Build message
    $values = array(
      'arguments' => array(
        '@entity_label' => is_object($entity) ? entity_label($entity_type, $entity) : "Deleted Entity",
      ),
      'timestamp' => $log_message['timestamp'],
    );

    $message = message_create($type, $values, $log_message['account']);
    $wrapper = entity_metadata_wrapper('message', $message);
    $wrapper->party = $controller->getParty();
    if (is_object($entity)) {
      $wrapper->{$field_name} = $entity;
    }
    $wrapper->save();
  }
}
