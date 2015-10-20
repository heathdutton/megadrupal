<?php

/**
 * @file
 * Custom services definition and implementation of all callbacks.
 */

/**
 * Provides API definition of provided services objects and operations.
 */
function _services_raw_services_resources() {
  $services = array();

  // Node
  $services['node_raw'] = array(
    'create' => array(
      'help' => 'Creates a node using direct node_save call.',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'callback' => '_services_raw_node_create',
      'args' => array(
         array(
          'name' => 'node',
          'type' => 'struct',
          'description' => 'The node object',
          'source' => 'data',
          'optional' => FALSE,
        ),
      ),
      'access callback' => '_services_raw_node_access',
      'access arguments' => array('create'),
      'access arguments append' => TRUE,
    ),
    'update' => array(
      'help' => 'Update a node using direct node_save call.',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'callback' => '_services_raw_node_update',
      'args' => array(
        array(
          'name' => 'nid',
          'optional' => FALSE,
          'source' => array('path' => 0),
          'type' => 'int',
          'description' => 'The nid of the node to get',
        ),
        array(
          'name' => 'node',
          'optional' => FALSE,
          'source' => 'data',
          'description' => 'The node data to update',
          'type' => 'array',
        ),
      ),
      'access callback' => '_services_raw_node_access',
      'access arguments' => array('update'),
      'access arguments append' => TRUE,
    ),
    'targeted_actions' => array(
      'attach_file' => array(
        'help' => 'Attach file to the specific node field.',
        'file' => array('type' => 'inc', 'module' => 'services_raw'),
        'callback' => '_services_raw_node_attach_file',
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to attach a file to',
          ),
          array(
            'name' => 'field_name',
            'optional' => TRUE,
            'source' => array('data' => 'field_name'),
            'description' => 'Field name for attached files',
            'type' => 'string',
          ),
          array(
            'name' => 'fid',
            'optional' => TRUE,
            'source' => array('data' => 'fid'),
            'description' => 'File fid of the file to be attached',
            'type' => 'int',
          ),
        ),
        'access callback' => '_services_raw_node_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      )
    )
  );

  // Field
  $services['field'] = array(
    'update' => array(
      'help' => 'Update a field using entity-appropriate functions.',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'callback' => '_services_raw_field_update',
      'args' => array(
        'uuid' => array(
          'name' => 'uuid',
          'optional' => FALSE,
          'source' => array('path' => 0),
          'type' => 'string',
          'description' => 'The UUID of the object to update',
        ),
        'field' => array(
          'name' => 'field',
          'optional' => FALSE,
          'source' => array('data' => 'field'),
          'description' => 'The field name to update (will use current node/field lang)',
          'type' => 'string',
        ),
        'entity_type' => array(
          'name' => 'entity_type',
          'optional' => FALSE,
          'source' => array('data' => 'entity_type'),
          'description' => 'The entity type to update (node, user, etc.)',
          'type' => 'string',
        ),
        'value'=> array(
          'name' => 'value',
          'optional' => FALSE,
          'source' => array('data' => 'value'),
          'description' => 'The field value to update.',
          'type' => 'array',
        ),
      ),
      'access callback' => '_services_raw_field_access',
      'access arguments' => array('update'),
      'access arguments append' => TRUE,
    ),
  );

  // Fields
  $services['fields'] = array(
    'update' => array(
      'help' => 'Update a set of fields using entity-appropriate functions.',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'callback' => '_services_raw_fields_update',
      'args' => array(
        'uuid' => array(
          'name' => 'uuid',
          'optional' => FALSE,
          'source' => array('path' => 0),
          'type' => 'string',
          'description' => 'The UUID of the object to update',
        ),
        'entity_type' => array(
          'name' => 'entity_type',
          'optional' => FALSE,
          'source' => array('data' => 'entity_type'),
          'description' => 'The entity type to update (node, user, etc.)',
          'type' => 'string',
        ),
        'data' => array(
          'name' => 'data',
          'optional' => FALSE,
          'source' => array('data' => 'data'),
          'description' => 'An array with the field name as key and the value as array value',
          'type' => 'array',
        ),
      ),
      'access callback' => '_services_raw_field_access',
      'access arguments' => array('update'),
      'access arguments append' => TRUE,
    ),
  );

  // User
  $services['user_raw'] = array(
    'retrieve' => array(
      'help' => 'Retrieves a user',
      'callback' => '_services_raw_user_retrieve',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'access callback' => '_services_raw_user_access',
      'access arguments' => array('view'),
      'access arguments append' => TRUE,
      'args' => array(
        array(
          'name' => 'uid',
          'type' => 'int',
          'description' => 'The uid of the user to retrieve.',
          'source' => array('path' => '0'),
          'optional' => FALSE,
        ),
      ),
    ),
    'create' => array(
      'help' => 'Creates a user using direct user_save',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'callback' => '_services_raw_user_create',
      'access callback' => '_services_raw_user_access',
      'access arguments' => array('create'),
      'access arguments append' => FALSE,
      'args' => array(
        array(
          'name' => 'account',
          'type' => 'array',
          'description' => 'The user object',
          'source' => 'data',
          'optional' => FALSE,
        ),
      ),
    ),

    'update' => array(
      'help' => 'Updates a user using direct user_save',
      'file' => array('file' => 'inc', 'module' => 'services_raw'),
      'callback' => '_services_raw_user_update',
      'access callback' => '_services_raw_user_access',
      'access arguments' => array('update'),
      'access arguments append' => TRUE,
      'args' => array(
        array(
          'name' => 'uid',
          'type' => 'int',
          'description' => 'Unique identifier for this user',
          'source' => array('path' => 0),
          'optional' => FALSE,
        ),
        array(
          'name' => 'data',
          'type' => 'array',
          'description' => 'The user object with updated information',
          'source' => 'data',
          'optional' => FALSE,
        ),
      ),
    ),
  );

  return $services;
}

//
// Node
//

/**
 * Determine whether user has access to node resource and raw node_save.
 *
 * @param $op
 *   Operation
 * @param $args
 *   Additional arguments passed to call
 */
function _services_raw_node_access($op = 'view', $args = array()) {
  // Load original node resource
  module_load_include('inc', 'services', 'resources/node_resource');
  // Check access
  return user_access('access raw node_save') && _node_resource_access($op, $args);
}

/**
 * Determine whether user has access to node resource and raw node_save.
 *
 * @param $op
 *   Operation
 * @param $args
 *   Additional arguments passed to call
 */
function _services_raw_field_access($op = 'view', $args = array()) {
  switch ($args['entity_type']) {
    case 'node':
      module_load_include('inc', 'services', 'resources/node_resource');
      if (function_exists('entity_get_id_by_uuid')) {
        $result = entity_get_id_by_uuid('node', array($args['uuid']));
        $args[0]['nid'] = reset($result);
      }
      else {
        $args[0]['nid'] = uuid_node_find($args['uuid']);
      }
      return user_access('access raw node_save') && _node_resource_access($op, $args);

    case 'user':
      module_load_include('inc', 'services', 'resources/user_resource');
      if (function_exists('entity_get_id_by_uuid')) {
        $result = entity_get_id_by_uuid('user', $args['uuid']);
        $args[0]['uid'] = reset($result);
      }
      else {
        $args[0]['uid'] = uuid_user_find($args['uuid']);
      }
      return user_access('access raw user_save') && _node_resource_access($op, $args);

    // This could be expanded to allow for other entity types as this module
    // develops to support them. For now we can only dance with nodes and users.
    default:
      return FALSE;
  }
}

/**
 * Update entity calling array of _save functions.
 *
 * @param $uuid
 *   UUID of saved entity
 * @param $field
 *   Field of entity that needs to be saved
 * @param $entity_type
 *   Currently node or user.
 * @param $value
 *   Array of field values that needs to be saved
 */
function _services_raw_field_update($uuid, $field, $entity_type, $value) {
  switch ($entity_type) {
    case 'node':
      $id_name = 'nid';
      if (function_exists('entity_get_id_by_uuid')) {
        $result = entity_get_id_by_uuid('node', array($uuid));
        $id = reset($result);
      }
      else {
        $id = uuid_node_find($uuid);
      }
      $save_functions = array('node_object_prepare', 'node_save');
      break;

    case 'user':
      $id_name = 'uid';
      if (function_exists('entity_get_id_by_uuid')) {
        $result = entity_get_id_by_uuid('user', $uuid);
        $id = reset($result);
      }
      else {
        $id = uuid_user_find($uuid);
      }
      $save_functions = array('user_save');
      break;

    default:
      return services_error(t('Entity type @type not supported', array('@type' => $entity_type)), 404);
  }

  module_load_include('inc', 'services', 'resources/' . $entity_type . '_resource');

  $entity_list = entity_load($entity_type, array($id));
  if (is_array($entity_list)) {
    $entity = reset($entity_list);
  }
  if (empty($entity->{$id_name})) {
    return services_error(t('@type @id not found', array('@type' => $entity_type, '@id' => $id)), 404);
  }

  // Validate that the passed field is on this entity
  $fields_info = field_info_instances($entity_type, $entity->type);
  $match = FALSE;
  foreach ($fields_info as $field_name => $field_info) {
    if ($field_name == $field) {
      $match = TRUE;
      continue;
    }
  }
  if (!$match) {
    return services_error(t('Field @field not found', array('@field' => $field)), 404);
  }

  try {
    // Prepare entity defaults
    $lang = field_language($entity_type, $entity, $field_name);
    $i = 0;
    $field_value = NULL;
    foreach($value as $val) {
      $field_value[$i]['value'] = $val;
      $i++;
    }
    $entity->{$field}[$lang] = $field_value;
    $entity->_services_client['visted'][] = services_client_get_id();
    foreach ($save_functions as $function) {
      $function($entity);
    }

    if (empty($entity->{$id_name})) {
      return services_error(t('Error when saving entity.'), 406);
    }
  }
  catch (Exception $e) {
    return services_error(t('Error when saving entity.'), 406, array('error' => $e->getMessage()));
  }

  $result = array($id => $entity->{$id_name});
  if (($uri = services_resource_uri(array($entity_type, $entity->{$id_name})))) {
    $result['uri'] = $uri;
  }
  return $result;
}

/**
 * Update entity calling array of _save functions.
 *
 * @param $uuid
 *   UUID of saved entity
 * @param $entity_type
 *   Currently node or user.
 * @param $field
 *   Field of entity that needs to be saved
 * @param $data
 *   Array of fields with their respective values that needs to be saved
 */
function _services_raw_fields_update($uuid, $entity_type, $data) {
  switch ($entity_type) {
    case 'node':
      $id_name = 'nid';
      if (function_exists('entity_get_id_by_uuid')) {
        $result = entity_get_id_by_uuid('node', array($uuid));
        $id = reset($result);
      }
      else {
        $id = uuid_node_find($uuid);
      }

      $save_functions = array('node_object_prepare', 'node_save');
      break;

    case 'user':
      $id_name = 'uid';
      if (function_exists('entity_get_id_by_uuid')) {
        $result = entity_get_id_by_uuid('user', $uuid);
        $id = reset($result);
      }
      else {
        $id = uuid_user_find($uuid);
      }
      $save_functions = array('user_save');
      break;

    default:
      return services_error(t('Entity type @type not supported', array('@type' => $entity_type)), 404);
  }

  module_load_include('inc', 'services', 'resources/' . $entity_type . '_resource');

  $entity_list = entity_load($entity_type, array($id));
  if (is_array($entity_list)) {
    $entity = reset($entity_list);
  }
  if (empty($entity->{$id_name})) {
    return services_error(t('@type @id not found', array('@type' => $entity_type, '@id' => $id)), 404);
  }

  // Get all the fields for this entity type
  $fields_info = field_info_instances($entity_type, $entity->type);

  // Loop over all the new updates
  foreach ($data as $new_field => $new_field_values) {

    // Validate that the passed field is on this entity
    $match = FALSE;
    foreach (array_keys($fields_info) as $cur_field) {
      if ($cur_field == $new_field) {
        $match = TRUE;
        continue;
      }
    }

    // We need to find all fields that were given
    if ($match == FALSE) {
      return services_error(t('Field @field not found', array('@field' => $new_field)), 404);
    }

    // Prepare entity defaults
    $lang = field_language($entity_type, $entity, $new_field);
    // Get field values in a numeric list
    $i = 0;
    $field_value = NULL;
    foreach($new_field_values as $new_field_value) {
      $field_value[$i]['value'] = $new_field_value;
      $i++;
    }
    $entity->{$new_field}[$lang] = $field_value;
  }

  // Register where we come from
  $entity->_services_client['visted'][] = services_client_get_id();

  try {
    // Save the data
    foreach ($save_functions as $function) {
      $function($entity);
    }

    // If the identifier cannot be found, quit with a notice
    if (empty($entity->{$id_name})) {
      return services_error(t('Error when saving entity.'), 406);
    }
  }
  catch (Exception $e) {
    return services_error(t('Error when saving entity.'), 406, array('error' => $e->getMessage()));
  }

  $result = array($id => $entity->{$id_name});
  if (($uri = services_resource_uri(array($entity_type, $entity->{$id_name})))) {
    $result['uri'] = $uri;
  }
  return $result;
}

/**
 * Update node calling raw node_save.
 *
 * @param $nid
 *   Nid of saved node
 * @param $node
 *   Array of node that needs to be saved
 * @return array|mixed
 */
function _services_raw_node_update($nid, $node) {
  // Load original node resource
  module_load_include('inc', 'services', 'resources/node_resource');

  // Adds backwards compatibility with regression fixed in #1083242
  $node = _services_arg_value($node, 'node');

  $node['nid'] = $nid;

  $old_node = node_load($nid);
  if (empty($old_node->nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $nid)), 404);
  }

  // Add missing attributes from original node
  $node += (array) $old_node;

  // If revision is not included load revision of existing node
  if (empty($node['vid']) && $old_node->vid) {
    $node['vid'] = $old_node->vid;
  }

  // Assign username to the node from $user created at auth step.
  if (isset($node['name']) && !isset($node['uid'])) {
    $account = user_load_by_name($node['name']);
    if (!empty($account)) {
      $node['uid'] = $account->uid;
    }
    else {
      unset($node['name']);
    }
  }
  if (!isset($node['name']) && !isset($node['uid'])) {
    global $user;
    $node['name'] = $user->name;
    $node['uid'] = $user->uid;
  }

  // Validate the node. If there is validation error Exception will be thrown
  // so code below won't be executed.
  _node_resource_validate_type($node);

  // Check if call isn't trying to change node type
  if ($old_node->type != $node['type']) {
    return services_error(t("You can't change node type via services"), 406);
  }

  try {
    // Prepare node defaults
    $node = (object) $node;
    // Prevent loops
    // this is apparently over-zealous as it stands and prevents insight
    // from getting updated at all.
    //$node->_services_client['visted'][] = services_client_get_id();
    node_object_prepare($node);
    node_save($node);

    if (empty($node->nid)) {
      return services_error(t('Error when saving node.'), 406);
    }
  }
  catch (Exception $e) {
    return services_error(t('Error when saving node.'), 406, array('error' => $e->getMessage()));
  }

  $result = array('nid' => $node->nid);
  if (($uri = services_resource_uri(array('node', $node->nid)))) {
    $result['uri'] = $uri;
  }
  return $result;
}

/**
 * Create node calling raw node_save.
 *
 * @param $node
 *   Array of node that needs to be saved
 */
function _services_raw_node_create($node) {
  // Load original node resource
  module_load_include('inc', 'services', 'resources/node_resource');

  // Adds backwards compatibility with regression fixed in #1083242
  $node = _services_arg_value($node, 'node');

  // Assign username to the node from $user created at auth step.
  if (isset($node['name']) && !isset($node['uid'])) {
    $account = user_load_by_name($node['name']);
    if (!empty($account)) {
      $node['uid'] = $account->uid;
    }
    else {
      unset($node['name']);
    }
  }
  if (!isset($node['name']) && !isset($node['uid'])) {
    global $user;
    $node['name'] = $user->name;
    $node['uid'] = $user->uid;
  }

  // Validate the node. If there is validation error Exception will be thrown
  // so code below won't be executed.
  _node_resource_validate_type($node);

  // Keep original node sent from remote site
  $node_array = $node;
  $keep_properties = array('uid', 'created');

  // Prepare node object, ensure that we're not going to update some node.
  $node = (object) $node;
  $node->is_new = TRUE;
  unset($node->nid);
  node_object_prepare($node);
  node_submit($node);

  // Force properties from remote site like created, uid
  foreach ($keep_properties as $property) {
    if (isset($node_array[$property])) {
      $node->{$property} = $node_array[$property];
    }
  }

  node_save($node);

  if (!$node->nid) {
    return services_error(t('Error when saving node'), 406);
  }

  // Only add the URI for servers that support it.
  $result = array('nid' => $node->nid);
  if (($uri = services_resource_uri(array('node', $node->nid)))) {
    $result['uri'] = $uri;
  }
  return $result;
}

/**
 * Attach file object to node filefield
 *
 * @param $nid
 *   Node id
 * @param $field_name
 *   Field name to be updated with attached file
 * @param $fid
 *   File object id
 * @return array|mixed
 */
function _services_raw_node_attach_file($nid, $field_name, $fid) {
  // Check if uploaded file exists
  if ($file = file_load($fid)) {
    $file->display = 1;
    $file->description = '';

    $node = node_load($nid);
    if (!isset($node->{$field_name})) {
      // Do not allow to update not existing node field
      services_error(t("The field %field_name doesn't exist", array('%field_name' => $field_name)), 403);
    }
    $node->revision = '';
    $node->{$field_name}['und'][] = (array) $file;
    node_save($node);
  }
  else {
    return services_error(t("The file fid %fid could not be attached, because it doesn't exist.", array('%fid' => $fid)), 406);
  }

  // Only add the URI for servers that support it.
  $result = array('nid' => $node->nid);
  if (($uri = services_resource_uri(array('node', $node->nid)))) {
    $result['uri'] = $uri;
  }
  return $result;
}

//
// User
//

/**
 * Determine whether user has access to node resource and raw node_save.
 *
 * @param $op
 *   Operation
 * @param $args
 *   Additional arguments passed to call
 * @return bool
 */
function _services_raw_user_access($op = 'view', $args = array()) {
  // Load original node resource
  module_load_include('inc', 'services', 'resources/user_resource');
  // Check access
  $result = user_access('access raw user_save') && _user_resource_access($op, $args);
  return $result;
}

/**
 * Load user from database. This method don't filter user passwords
 *
 * @param $uid
 * @return mixed
 */
function _services_raw_user_retrieve($uid) {
  $account = user_load($uid);
  if (empty($account)) {
    return services_error(t('There is no user with ID @uid.', array('@uid' => $uid)), 404);
  }

  // Everything went right.
  return $account;
}

/**
 * Create new user account using user_save
 *
 * @param $account
 *   Account that should be created
 * @return array|mixed
 */
function _services_raw_user_create($account) {
  // Load original node resource
  module_load_include('inc', 'services', 'resources/user_resource');

  // Adds backwards compatibility with regression fixed in #1083242
  $account = _services_arg_value($account, 'account');

  // Prevent re-writing user
  unset($account['uid']);

  // Check if user email already exists
  $exists = db_select('users', 'u')
    ->fields('u', array('mail'))
    ->condition('u.mail', $account['mail'])
    ->countQuery()
    ->execute()
    ->fetchField();
  if ($exists) {
    return services_error(t("Error when saving user account."), 406, array(
      'error' => t('Account with specified email @mail already exists.', array(
        '@mail' => $account['mail']
       )
    )));
  }

  try {
    $account_raw = $account;
    if (!($account = user_save(NULL, $account))) {
      return services_error(t("Error when saving user account."), 406);
    }
    // Password is automatically rehashed by user module, manual sql update is required
    if (!empty($account_raw['pass'])) {
      db_update('users')
        ->fields(array('pass' => $account_raw['pass']))
        ->condition('uid', $account->uid, '=')
        ->execute();
    }
  }
  catch (Exception $e) {
    return services_error(t("Error when saving user account."), 406, array('error' => $e->getMessage()));
  }

  $result = array('uid' => $account->uid);
  if (($uri = services_resource_uri(array('user', $account->uid)))) {
    $result['uri'] = $uri;
  }
  return $result;
}

/**
 * Update user account using user_save
 *
 * @param $uid
 * @param $account
 *   Account that should be created
 * @return array|mixed
 */
function _services_raw_user_update($uid, $account) {
  // Adds backwards compatibility with regression fixed in #1083242
  $account = _services_arg_value($account, 'data');

  $original_account = user_load($uid);

  $account['uid'] = $uid;

  try {
    $account_raw = $account;
    if (!($account = user_save($original_account, $account))) {
      return services_error(t("Error when saving user account."), 406);
    }
    // Password is automatically rehashed by user module, manual sql update is required
    if (!empty($account_raw['pass'])) {
      db_update('users')
        ->fields(array('pass' => $account_raw['pass']))
        ->condition('uid', $account->uid, '=')
        ->execute();
    }
  }
  catch (Exception $e) {
    return services_error(t("Error when saving user account."), 406, array('error' => $e->getMessage()));
  }

  $result = array('uid' => $account->uid);
  if (($uri = services_resource_uri(array('user', $account->uid)))) {
    $result['uri'] = $uri;
  }
  return $result;
}

//
// Comment
//

//
// Taxonomy
//

//
// File
//
