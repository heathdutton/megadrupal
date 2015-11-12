<?php
/**
 * @file
 * Database related functions.
 */

/* PUBLIC KEYS. */

/**
 * Selects public key of an instance.
 *
 * @param int $instance_id
 *   The id of the instance
 *
 * @param int $public_key_id
 *   The id of the public key
 *
 * @return int
 *   The number of rows updated in the database. A successful operation
 *   should update only 1 row
 */
function d2d_api_public_key_select($instance_id, $public_key_id) {
  $num_updated = db_update('d2d_instances')
    ->fields(array('public_key_id' => $public_key_id))
    ->condition('id', $instance_id)
    ->execute();
  return $num_updated;
}

/**
 * Unselects public key of an instance.
 *
 * @param int $instance_id
 *   The id of the instance
 */
function d2d_api_public_key_unselect($instance_id) {
  return d2d_api_public_key_select($instance_id, NULL);
}

/**
 * Updates the row of the {d2d_public_keys} table with the given id.
 *
 * @param int $id
 *   The id of the instance
 *
 * @param array $update
 *   The associative array of fields to update
 *
 * @return int
 *   The number of updated rows (Should be 1 for a successfull operation)
 */
function d2d_api_public_key_update($id, $update) {
  $num_updated = db_update('d2d_public_keys')
    ->fields($update)
    ->condition('id', $id)
    ->execute();

  return $num_updated;
}

/**
 * Removes a public key from the database.
 *
 * @param int $id
 *   The id of the public key
 *
 * @return int
 *   The number of updated rows (Should be 1 for a successfull operation)
 */
function d2d_api_public_key_delete($id) {
  $num_deleted = db_delete('d2d_public_keys')
    ->condition('id', $id)
    ->execute();
  return $num_deleted;
}

/**
 * Insert a new row in the {d2d_public_keys} table.
 *
 * @param number $instance_id
 *   The database id of the instance
 * @param string $public_key
 *   The public key in plain text
 *
 * @return int
 *   The id of the newly inserted row
 */
function d2d_api_public_key_set($instance_id, $public_key) {
  $id_pk = db_insert('d2d_public_keys')->fields(array(
      'instance_id' => $instance_id,
      'public_key' => $public_key,
  ))->execute();
  return $id_pk;
}


/**
 * Returns a query object that joins d2d_public_keys and d2d_instances tables.
 *
 * This function should not be called directly.
 */
function _d2d_api_public_key_get_join_query() {
  $query = db_select('d2d_public_keys', 'k');

  $query->join('d2d_instances', 'i', 'i.id = k.instance_id');
  $query
  ->fields('i', array('d2d_id'))
  ->fields('k', array('id', 'instance_id', 'public_key'));

  return $query;
}

/**
 * Retrieves currently selected public key objects from the database.
 *
 * @return array|stdClass
 *   The result set
 */
function d2d_api_public_key_get_selected() {
  $query = _d2d_api_public_key_get_join_query();
  return $query->execute()->FetchAll();
}

/**
 * Retrieves public key objects from the database.
 *
 * If the d2d_id parameter is specified, it returns only the keys
 * for the instance with the given d2d_id.
 *
 * @param string $d2d_id
 *   Optional. Restricts the result set to keys belonging to this instance
 *
 * @return array|stdClass
 *   The result set
 */
function d2d_api_public_key_get($d2d_id = NULL) {
  $query = _d2d_api_public_key_get_join_query();
  if (!is_null($d2d_id)) {
    $query->condition('i.d2d_id', $d2d_id);
  }
  return $query->execute()->FetchAll();
}


/**
 * Retrieves the public key with the given id.
 *
 * @param int $id
 *   The id of the public key
 *
 * @return stdClass
 *   The public key object as fetched from the database
 */
function d2d_api_public_key_get_by_id($id) {
  $query = db_select('d2d_public_keys', 'k')
              ->fields('k')
              ->condition('k.id', $id);
  return $query->execute()->FetchObject();
}


/**
 * Retrieves the numeric id of a public key.
 *
 * Public key is identified by the pair d2d_id and public key plain text.
 *
 * @param string $d2d_id
 *   The d2d id of an instance in the database
 * @param string $public_key
 *   The plain text public key
 *
 * @return bool|integer
 *   The public key id, or FALSE if an error occurs
 *
 * @see d2d_public_key_equal()
 */
function d2d_api_public_key_return_id($d2d_id, $public_key) {
  $keys = d2d_api_public_key_get($d2d_id);
  foreach ($keys as $k) {
    if (d2d_public_key_equal($k->public_key, $public_key)) {
      return $k->id;
    }
  }
  return FALSE;
}

/**
 * Checks if specified public key for an instance already exists.
 *
 * @param string $d2d_id
 *   The d2d_id of the instance
 *
 * @param string $public_key
 *   The id of the public key
 *
 * @return bool
 *   TRUE, if the key  is associated with the instance with the specified d2d_id
 */
function d2d_api_public_key_exist($d2d_id, $public_key) {
  return d2d_api_public_key_return_id($d2d_id, $public_key) !== FALSE;
}

/**
 * Checks if specified public key for an instance already exists.
 *
 * @param array $instance
 *   The associative array of the instance containing keys 'd2d_id' and 'url'.
 *
 * @param bool $verbose
 *   Optional. If TRUE, uses drupal_set_message to display the results
 *   of the operation, Defaults, FALSE
 *
 * @return string|bool
 *   The plain text public key for the requested instance, or FALSE
 *   if an error occurs.
 */
function d2d_api_public_key_request($instance, $verbose = FALSE) {

  $res = xmlrpc($instance['url'], array('d2d.getPublicKey' => array($instance['d2d_id'])));
  if ($verbose) {
    if ($res === FALSE) {
      $xmlrpc_error = xmlrpc_error();
      if ($xmlrpc_error != NULL) {
        drupal_set_message(t('Error retrieving public key.<br/>Error code @code<br/>"@message"',
            array('@code' => $xmlrpc_error->code, '@message' => $xmlrpc_error->message)), 'error');
      }
      else {
        drupal_set_message(t('Error retrieving public key.'), 'error');
      }
    }
    else {
      drupal_set_message(t('Public key has been received.'));
    }
  }
  return $res;
}

/* INSTANCES. */

/**
 * Removes an instance and all associated entries from the database.
 *
 * @param int $instance_id
 *   The id of the instance
 *
 * @param bool $notify
 *   Optional. If TRUE, sends a notification of friendship update to
 *   the deleted instance
 *
 * @param bool $verbose
 *   Optional. If TRUE, uses drupal_set_message to display the results
 *   of the operation
 *
 * @return bool
 *   TRUE, if the key  is associated with the instance with the specified d2d_id
 *
 * @see d2d_api_friendship_send()
 * @see d2d_send_outgoing_requests()
 */
function d2d_api_instance_delete($instance_id, $notify = FALSE, $verbose = FALSE) {
  $instance = d2d_api_instance_get_by_id($instance_id);
  // Remove friendship.
  d2d_api_friendship_update($instance_id, 0);
  $return = $notify ? d2d_api_friendship_send($instance_id, TRUE, 1, $verbose) : NULL;
  // Remove group memberships.
  db_delete('d2d_group_memberships')
  ->condition('instance_id', $instance_id)
  ->execute();
  // Remove public keys associated with this instance.
  db_delete('d2d_public_keys')
  ->condition('instance_id', $instance_id)
  ->execute();
  // Remove incoming requests.
  db_delete('d2d_incoming_requests')
  ->condition('sender_d2d_id', $instance['d2d_id'])
  ->execute();
  // Remove outgoing requests.
  db_delete('d2d_outgoing_requests')
  ->condition('receiver_id', $instance_id)
  ->execute();
  // Remove instance.
  db_delete('d2d_instances')
  ->condition('id', $instance_id)
  ->execute();
  // Call hook.
  module_invoke_all('d2d_instance_removed', $instance);
  return $return;
}

/**
 * Creates a new instance in the database.
 *
 * @param array $instance
 *   Associative array describing the instance to be inserted into the db.
 *
 * @param bool $set_default_group_membership
 *   Optional. If TRUE, the instance is added to the default group.
 *
 * @return int
 *   The id in the database (not the d2d_id)
 */
function d2d_api_instance_add($instance, $set_default_group_membership = TRUE) {
  $id = db_insert('d2d_instances')->fields(array(
    'd2d_id' => $instance['d2d_id'],
    'name' => $instance['name'],
    'url' => $instance['url'],
    'description' => $instance['description'],
    'time_inserted' => d2d_get_time(),
    'public_key_id' => NULL,
  ))->execute();
  if ($set_default_group_membership) {
    d2d_set_default_group_memberships($id);
  }
  return $id;
}

/* OWN PUBLIC KEY. */

/**
 * Retrieves own public key (plain text).
 *
 * @return string
 *   The public key
 */
function d2d_api_own_public_key_get() {
  $my_d2d_id = d2d_api_own_d2d_id_get();
  $key = d2d_api_public_key_get($my_d2d_id);
  // TODO: this is correct if at most one candidate key for own instance
  // exists, would be buggy if you mess around and this invariant does not hold.
  return empty($key) ? NULL : $key[0]->public_key;
}

/**
 * Sets own public key (plain text).
 *
 * Delete all others public keys associated with own instance.
 *
 * @param string $public_key
 *   The public key plain text
 *
 * @return int
 *   The public key id
 */
function d2d_api_own_public_key_set($public_key) {
  $id = d2d_api_own_instance_id_get();
  $id_pk = d2d_api_public_key_set($id, $public_key);
  d2d_api_instance_update($id, array(
      'public_key_id' => $id_pk,
  ));

  db_delete('d2d_public_keys')
    ->condition('id', $id_pk, '<>')
    ->condition('instance_id', $id)
    ->execute();

  return $id_pk;
}

/* OWN PRIVATE KEY. */

/**
 * Returns own instance private key.
 */
function d2d_api_own_private_key_get() {
  return variable_get('d2d_private_key', NULL);
}

/**
 * Sets own instance private key.
 */
function d2d_api_own_private_key_set($private_key) {
  variable_set('d2d_private_key', $private_key);
}

/**
 * Delete own instance private key.
 *
 * Notice: this just deletes a reference in the table variable
 */
function d2d_api_own_private_key_delete() {
  variable_del('d2d_private_key');
}

/* OWN D2D_ID. */

/**
 * Returns own instance d2d_id.
 */
function d2d_api_own_d2d_id_get() {
  return variable_get('d2d_my_id', NULL);
}

/**
 * Sets own instance d2d_id.
 *
 * When this is used, the own_instance_id should probably be updated too.
 */
function d2d_api_own_d2d_id_set($d2d_id) {
  variable_set('d2d_my_id', $d2d_id);
}

/**
 * Deletes own instance d2d_id.
 *
 * Note: this just deletes a reference in the table variable
 */
function d2d_api_own_d2d_id_delete() {
  variable_del('d2d_my_id');
}

/* OWN INSTANCE_ID. */

/**
 * Returns own instance database id.
 */
function d2d_api_own_instance_id_get() {
  return variable_get('d2d_my_instance_id', NULL);
}

/**
 * Sets own instance database id.
 *
 * When this is used, the own_instance_d2d_id should probably be updated too.
 */
function d2d_api_own_instance_id_set($id) {
  variable_set('d2d_my_instance_id', $id);
}

/**
 * Deletes own instance database id.
 *
 * Notice: this just deletes a reference in the table variable
 */
function d2d_api_own_instance_id_delete() {
  variable_del('d2d_my_instance_id');
}


/* OWN INSTANCE. */

/**
 * Deletes own instance from database.
 *
 * This should only be called only in offline mode.
 */
function d2d_api_own_instance_delete() {
  $myid = d2d_api_own_instance_id_get();
  if (is_null($myid)) {
    return FALSE;
  }
  d2d_api_instance_delete($myid, FALSE, FALSE);
  d2d_api_own_private_key_delete();
  d2d_api_own_instance_id_delete();
  d2d_api_own_d2d_id_delete();
  menu_rebuild();
  return TRUE;
}

/**
 * Returns an associative array with data about own instance.
 *
 * @param bool $private_key
 *   Optional. TRUE, if you want to include the private key in the associative
 *   array
 */
function d2d_api_own_instance_get($private_key = FALSE) {
  $id = d2d_api_own_instance_id_get();
  $own = d2d_api_instance_get_by_id($id);
  if ($private_key) {
    $own['private_key'] = d2d_api_own_private_key_get();
  }
  return $own;
}

/* INSTANCES. */

/**
 * Low level primitive to load one or more instances from the database.
 *
 * @param int $id
 *   Optional. If set load just the instance with this id
 *
 * @param bool $id_as_d2d_id
 *   Optional. If TRUE, treats the id as a d2d_id, and returns
 *   an associative array ofinstances with the d2d_id as identifier.
 *
 * @return array|bool
 *   The list of instances, or FALSE if the loading failed
 */
function _d2d_api_instance_get($id, $id_as_d2d_id = FALSE) {

  $query = db_select('d2d_instances', 'i');
  $query->leftJoin('d2d_public_keys', 'k', 'i.public_key_id = k.id');
  $query
    ->fields('k', array('public_key'))
    ->fields('i');
  // Returns only one instance.
  if (!is_null($id)) {
    $query->condition($id_as_d2d_id ? 'i.d2d_id' : 'i.id', $id);
    $result = $query->execute();
    $instance = $result->fetchAssoc();
    if (empty($instance)) {
      return FALSE;
    }
    _d2d_decorate_instance_array($instance);
    return $instance;
  }
  $result = $query->execute();
  return _d2d_process_result_set_instance($result, $id_as_d2d_id);
}

/**
 * Returns a list of d2d instances or a single instance.
 *
 * The d2d_id of the instance is used as associative key
 *
 * If an id is set, only the instance with d2d_id equal to id
 * is returned.
 *
 * @param string $d2d_id
 *   Optional. The d2d_id of the instance
 *
 * @return array|bool
 *   The list of instances, or FALSE if loading failed
 *
 * @see _d2d_api_instance_get()
 */
function d2d_api_instance_get($d2d_id = NULL) {
  return _d2d_api_instance_get($d2d_id, TRUE);
}

/**
 * Returns a list of d2d instances or a single instance.
 *
 * The database id of the instance is used as associative key.
 *
 * If an id is set, only the instance with database id equal to id
 * is returned.
 *
 * @param int $id
 *   Optional. The id of the instance
 *
 * @return array|bool
 *   The list of instances, or FALSE if loading failed
 *
 * @see _d2d_api_instance_get()
 */
function d2d_api_instance_get_by_id($id = NULL) {
  return _d2d_api_instance_get($id, FALSE);
}

/**
 * Updates fields of an instance.
 *
 * @param int $instance_id
 *   The id of the instance
 *
 * @param array $fields
 *   Associative array containing the updated values
 *
 * @return int
 *   The number of instances modified (should be 1, if
 *   operation is successful)
 */
function d2d_api_instance_update($instance_id, $fields) {
  $num_updated = db_update('d2d_instances')
  ->fields($fields)
  ->condition('id', $instance_id)
  ->execute();
  return $num_updated;
}

/**
 * Returns TRUE if there is an instance without a public key.
 */
function d2d_api_instance_without_public_key_exists() {
  $query = db_select('d2d_instances', 'i');
  $query->leftJoin('d2d_public_keys', 'k', 'i.public_key_id = k.id');
  $query->isNull('public_key');
  $query->fields('i');
  $result = $query->execute()->fetchObject();
  return !empty($result);
}

/* FRIENDS. */

/**
 * Low level api to load all the friend instances from the database.
 *
 * @param bool $d2d_id
 *   If TRUE, returns an associative array of instances
 *   with the d2d_id as identifier.
 *
 * @return array
 *   Associative array of friend instances
 *
 * @see _d2d_process_result_set_instance()
 */
function _d2d_api_friend_get($d2d_id) {

  $query = db_select('d2d_instances', 'i');
  $query->leftJoin('d2d_public_keys', 'k', 'i.public_key_id = k.id');
  $query = $query
            ->fields('i')
            ->fields('k', array('public_key'))
            ->condition('i.friendship_state', 3);

  $result = $query->execute();
  $friends = _d2d_process_result_set_instance($result, $d2d_id);

  return $friends;
}

/**
 * Returns all the friend instances from the database.
 *
 * @return array
 *   Associative array of friend instances, with the d2d-id of the instances
 *   used as identifier.
 *
 * @see _d2d_api_friend_get()
 */
function d2d_api_friend_get() {
  return _d2d_api_friend_get(TRUE);
}

/**
 * Returns all the friend instances from the database.
 *
 * @return array
 *   Associative array of friend instances, with the database id of ther
 *   instances used as identifier.
 *
 * @see _d2d_api_friend_get()
 */
function d2d_api_friend_get_by_id() {
  return _d2d_api_friend_get(FALSE);
}

/**
 * Returns the friend instance with the specified url.
 *
 * @param string $url
 *   The url of the friend instance
 *
 * @return array
 *   The requested friend instances, or FALSE if none is found.
 *
 * @see _d2d_api_friend_get()
 */
function d2d_api_friend_get_by_url($url) {
  $friends = d2d_api_friend_get();
  foreach ($friends as $friend) {
    if ($friend['url'] === $url) {
      return $friend;
    }
  }
  return FALSE;
}

/* FRIENDSHIP. */

/**
 * Updates the state of a friendship.
 *
 * @param int $friend_id
 *   The id of the friend instance
 *
 * @param int $friendship_state
 *   The new friendship state
 *
 * @param int $old_friendship_state
 *   Optional. The new friendship state
 *
 * @param int $sender_id
 *   Optional. The instance initiating the change in the friendship state
 *
 * @param array $friend_instance
 *   Optional. The full array of the friend instance. If given, it will be
 *   to the hook d2d_friendship_update. Defaults, NULL.
 */
function d2d_api_friendship_update($friend_id, $friendship_state, $old_friendship_state = NULL, $sender_id = NULL, $friend_instance = NULL) {
  db_update('d2d_instances')
    ->fields(array('friendship_state' => $friendship_state))
    ->condition('id', $friend_id)
    ->execute();
  // Call hook.
  module_invoke_all('d2d_friendship_update', $friend_id, $friendship_state, $old_friendship_state, $sender_id, $friend_instance);
}

/**
 * Adds and optionally sends out a friendship request.
 *
 * @param int $id
 *   The id of the receiving instance
 *
 * @param int $send_now
 *   If TRUE, the request is immediately sent
 *
 * @param int $max_number_of_resends
 *   Optional. The maximum number of attempts to send the request
 *
 * @param int $verbose
 *   Optional. If TRUE, results of the operation are displayed
 *    using drupal_set_message
 *
 * @return int|NULL
 *   The id of the outgoing request, or NULL if $send_now is set FALSE
 *
 * @see d2d_add_request()
 */
function d2d_api_friendship_send($id, $send_now, $max_number_of_resends = D2D_REQUEST_MAX_NUMBER_OF_RESENDS, $verbose = FALSE) {
  $request_id = d2d_add_request($id, 'd2d_friendship', array(), $max_number_of_resends, TRUE);
  if ($send_now) {
    return d2d_send_outgoing_requests(array($request_id), $verbose);
  }
  else {
    return NULL;
  }
}

/* GROUPS. */

/**
 * Returns the group with the specified id.
 *
 * @param int $group_id
 *   Optional. The id of the group to load, or NULL to load all
 *
 * @param int $auto_member_only
 *   Optional. If TRUE, only groups with the auto_membership flag are loaded
 *
 * @return array|NULL
 *   The id of the outgoing request, or NULL if $send_now is set FALSE
 */
function d2d_api_group_get($group_id = NULL, $auto_member_only = FALSE) {
  $query = db_select('d2d_groups', 'g')->fields('g');
  if ($auto_member_only) {
    $query = $query->condition('g.auto_member', 1);
  }
  if (is_null($group_id)) {
    $result = $query->execute();
    $groups = array();
    foreach ($result as $record) {
      $groups[$record->id] = array(
        'id' => $record->id,
        'name' => $record->name,
        'description' => $record->description,
        'auto_member' => $record->auto_member,
      );
    }
    return $groups;
  }
  else {
    $query = $query->condition('g.id', $group_id);
    $result = $query->execute();
    $group = $result->fetchAssoc();
    return $group ? $group : NULL;
  }
}

/**
 * Returns group with the specified name.
 *
 * @param string $group_name
 *   The name of the group
 *
 * @return array|NULL
 *   The group or NULL if no group is found
 */
function d2d_api_group_get_by_name($group_name) {
  $query = db_select('d2d_groups', 'g')->fields('g');
  $query = $query->condition('g.name', $group_name);
  $result = $query->execute();
  $group = $result->fetchAssoc();
  return $group ? $group : NULL;
}

/**
 * Removes the groups with specified id.
 *
 * @param int|array $group_ids
 *   The ids of the groups to delete
 *
 * @return int
 *   The number of groups effectively deleted.
 */
function d2d_api_group_delete($group_ids) {
  if (empty($group_ids)) {
    return 0;
  }
  else {
    return db_delete('d2d_groups')
      ->condition('id', $group_ids)
      ->execute();
  }
}

/**
 * Creates a new group.
 *
 * @param string $name
 *   The name of the group
 *
 * @param string $description
 *   The description of the group
 *
 * @param bool $auto_member
 *   If TRUE, new instances will be automatically added to this group
 *
 * @return int|NULL
 *   The id of the new group, or NULL if an error occurred
 */
function d2d_api_group_add($name, $description, $auto_member) {
  try {
    $id = db_insert('d2d_groups')->fields(
      array(
        'name' => $name,
        'description' => $description,
        'auto_member' => $auto_member ? 1 : 0,
      )
    )->execute();
    return $id;
  }
  catch (Exception $e) {
    return NULL;
  }
}

/**
 * Updates attributes of a group.
 *
 * @param int $id
 *   The group id
 *
 * @param string $name
 *   The name of the group
 *
 * @param string $description
 *   The description of the group
 *
 * @param bool $auto_member
 *   If TRUE, new instances will be automatically added to this group
 *
 * @return int
 *   The number of groups update (should be 1 if operation is successful)
 */
function d2d_api_group_update($id, $name, $description, $auto_member) {
  return db_update('d2d_groups')
  ->fields(array(
      'name' => $name,
      'description' => $description,
      'auto_member' => $auto_member ? 1 : 0,
  ))
  ->condition('id', $id)
  ->execute();
}

/* GROUP MEMBERSHIP. */

/**
 * Returns all groups with a flag indicating group membership of given instance.
 *
 * @param int $instance_id
 *   The instance id
 *
 * @return array
 *   The associative array of groups with the is_member flag
 */
function d2d_api_group_membership_get($instance_id) {
  $query = db_select('d2d_groups', 'g');
  $query->leftJoin('d2d_group_memberships', 'gm', '(g.id = gm.group_id AND gm.instance_id = :instance_id)', array(':instance_id' => $instance_id));
  $query = $query->fields('g')
  ->fields('gm', array('group_id'));
  $result = $query->execute();
  $groups = array();
  foreach ($result as $record) {
    $groups[$record->id] = array(
      'id' => $record->id,
      'name' => $record->name,
      'description' => $record->description,
      'auto_member' => $record->auto_member,
      'is_member' => !is_null($record->group_id),
    );
  }
  return $groups;
}

/**
 * Removes all instances from groups specified as argument.
 *
 * @param int|array $group_ids
 *   The name of the group
 *
 * @return int
 *   The number of memberships deleted (should be 1 if operation is successful)
 */
function d2d_api_group_membership_delete($group_ids) {
  if (empty($group_ids)) {
    return 0;
  }
  else {
    return db_delete('d2d_group_memberships')
    ->condition('group_id', $group_ids)
    ->execute();
  }
}

/**
 * Updates group memberships for the instance with given database id.
 *
 * @param int $instance_id
 *   The id of the instance
 *
 * @param array $group_ids
 *   Optional. The id of the groups to instance will be member of. If empty,
 *   all memberships will be removed.
 */
function d2d_api_group_membership_update($instance_id, $group_ids = array()) {
  db_delete('d2d_group_memberships')
  ->condition('instance_id', $instance_id)->execute();
  if (!empty($group_ids)) {
    d2d_api_group_membership_add($instance_id, $group_ids);
  }
  else {
    // Call hook.
    module_invoke_all('d2d_group_membership', $instance_id, $group_ids);
  }
}

/**
 * Adds group memberships for the instance with given database id.
 *
 * @param int $instance_id
 *   The id of the instance
 *
 * @param array $group_ids
 *   Optional. The id of the groups to instance will be member of. If empty,
 *   no operation is performed.
 */
function d2d_api_group_membership_add($instance_id, $group_ids = array()) {
  if (!empty($group_ids)) {
    $query = db_insert('d2d_group_memberships')->fields(array('group_id', 'instance_id'));
    foreach ($group_ids as $group_id) {
      $query->values(array('group_id' => $group_id, 'instance_id' => $instance_id));
    }
    $query->execute();
    // Call hook.
    module_invoke_all('d2d_group_membership', $instance_id, $group_ids);
  }
}

/* PERMISSIONS. */

/**
 * Returns the current permission table.
 *
 * @param bool $look_up_table
 *   Optional. If TRUE, an associative array is returned
 *
 * @return array
 *   The list/table of permissions
 */
function d2d_api_permission_get($look_up_table = FALSE) {
  $query = db_select('d2d_permissions', 'p')->fields('p');
  $result = $query->execute();
  $permissions = array();
  foreach ($result as $record) {
    $permissions[] = array(
      'group_id' => $record->group_id,
      'method' => $record->method,
    );
  }
  if (!$look_up_table) {
    return $permissions;
  }
  else {
    $permission_table = array();
    foreach ($permissions as $permission) {
      $permission_table[$permission['group_id']][$permission['method']] = 1;
    }
    return $permission_table;
  }
}

/**
 * Clears all available permissions.
 */
function d2d_api_permission_clear() {
  return db_delete('d2d_permissions')->execute();
}

/**
 * Adds permissions.
 *
 * @param array $permissions
 *   An array containing nested arrays of pairs group id-method nams
 */
function d2d_api_permission_add($permissions) {
  $query = db_insert('d2d_permissions')->fields(array('group_id', 'method'));
  foreach ($permissions as $permission) {
    $query->values($permission);
  }
  $query->execute();
}

/**
 * Removes all permissions for groups specified as argument.
 *
 * @param array $group_ids
 *   The ids of the groups from whom permissions must be removed
 *
 * @return int
 *   The number of rows effectively deleted from the <d2d_permissions> table
 */
function d2d_api_permission_delete($group_ids) {
  if (empty($group_ids)) {
    return 0;
  }
  else {
    return db_delete('d2d_permissions')
      ->condition('group_id', $group_ids)
      ->execute();
  }
}

/**
 * Checks if instance may call a mathod if it is a friend.
 */
function d2d_api_method_permission_exists($instance_id, $method) {
  $query = db_select('d2d_group_memberships', 'gm');
  $query = $query->condition('gm.instance_id', $instance_id);
  $query->join('d2d_permissions', 'p', '(gm.group_id = p.group_id AND p.method = :method)', array(':method' => $method));
  $query = $query->fields('p', array('method'));
  $result = $query->execute();
  return $result->fetchAssoc() ? TRUE : FALSE;
}

/* NOTIFICATIONS. */

/**
 * Counts the number of unread notifications.
 */
function d2d_api_notification_count_unread() {
  // TODO: Some of the notifications might not have a callback that allows to
  // view them, hence they are counted as unread but also are not listed.
  $query = db_select('d2d_notifications', 'n');
  $query = $query->condition('n.seen', 0);
  $query = $query->fields('n', array('id'));
  $result = $query->execute();
  return $result->rowCount();
}

/**
 * Inserts a notification into the database.
 *
 * @param string $type
 *   An alphanumeric description of the category of the notification
 *
 * @param string $d2d_id
 *   The d2d id of the instance creating the notification
 *
 * @param string $content
 *   The text of the notification
 */
function d2d_api_notification_insert($type, $d2d_id, $content) {
  db_insert('d2d_notifications')
    ->fields(array('type', 'd2d_id', 'content', 'timestamp'))
    ->values(
      array(
        'type' => $type,
        'd2d_id' => $d2d_id,
        'content' => $content,
        'timestamp' => d2d_get_time(),
      )
    )
    ->execute();
}

/**
 * Marks a notification as read or unread.
 *
 * @param int $notification_id
 *   The id of the notification
 *
 * @param bool $seen
 *   If TRUE, marks the notification as read, FALSE for unread
 */
function d2d_api_notification_change_read($notification_id, $seen) {
  return db_update('d2d_notifications')
    ->fields(array(
      'seen' => $seen ? 1 : 0,
    ))
    ->condition('id', $notification_id)
    ->execute();
}

/**
 * Deletes a notification.
 *
 * @param int $notification_id
 *   The id of the notification
 */
function d2d_api_notification_delete($notification_id) {
  return db_delete('d2d_notifications')
    ->condition('id', $notification_id)
    ->execute();
}

/**
 * Returns notification of specified type(s).
 *
 * @param string|array $types
 *   The types of the notifications we are interested in
 *
 * @return array
 *   The associative array of notifications
 */
function d2d_api_notification_get_by_type($types) {
  $query = db_select('d2d_notifications', 'n');
  if (!is_null($types)) {
    if (empty($types)) {
      return array();
    }
    $query = $query->condition('n.type', $types);
  }
  $query = $query->fields('n')->orderBy('n.id', 'DESC');
  $result = $query->execute();
  $notifications = array();
  foreach ($result as $record) {
    $notifications[] = array(
      'id' => $record->id,
      'type' => $record->type,
      'd2d_id' => $record->d2d_id,
      'content' => $record->content,
      'timestamp' => $record->timestamp,
      'seen' => $record->seen,
    );
  }
  return $notifications;
}

/**
 * Returns a notification by its id.
 *
 * @param int $notification_id
 *   The notification id
 *
 * @return array|NULL
 *   The requested notification, or NULL if none was found
 */
function d2d_api_notification_get($notification_id) {
  $query = db_select('d2d_notifications', 'n');
  $query = $query->condition('n.id', $notification_id);
  $query = $query->fields('n');
  $result = $query->execute();
  $notification = $result->fetchAssoc();
  return $notification ? $notification : NULL;
}

/* METHODS. */

/**
 * Returns a list of methods an instance may call if it is a friend.
 *
 * @param int $instance_id
 *   The id of the instance
 *
 * @param bool $look_up_table
 *   If TRUE, returns an associative array
 *
 * @return array
 *   The list/table of methods available to be called
 */
function d2d_api_method_get($instance_id, $look_up_table = FALSE) {
  $query = db_select('d2d_group_memberships', 'gm');
  $query = $query->condition('gm.instance_id', $instance_id);
  $query->join('d2d_permissions', 'p', 'gm.group_id = p.group_id');
  $query = $query->fields('p', array('method'));
  $query = $query->distinct();
  $result = $query->execute();
  if ($look_up_table) {
    $method_table = array();
    foreach ($result as $record) {
      $method_table[$record->method] = 1;
    }
    return $method_table;
  }
  else {
    $methods = array();
    foreach ($result as $record) {
      $methods[] = $record->method;
    }
    return $methods;
  }
}

/**
 * Returns TRUE if the d2d_id for own istance has been set.
 */
function d2d_api_initialization_is_completed() {
  return !is_null(d2d_api_own_d2d_id_get());
}

/**
 * Returns outgoing requests.
 *
 * @param array $types
 *   Optional. If set, only requests included in $types are returned.
 *
 * @return array
 *   The array of outgoing requests
 */
function d2d_api_outgoing_request_get($types) {
  if (is_array($types) && empty($types)) {
    return array();
  }
  $query = db_select('d2d_outgoing_requests', 'o');
  $query = $query->fields('o');
  if (!is_null($types)) {
    $query = $query->condition('o.type', $types);
  }
  $query = $query->orderBy('o.id', 'DESC');
  $result = $query->execute();
  return $result;
}

/**
 * Deletes an outgoing request.
 *
 * @param int $id
 *   The id of the outgoing request to delete
 *
 * @return int
 *   The number of outgoing request effectively deleted
 */
function d2d_api_outgoing_request_delete($id) {
  return db_delete('d2d_outgoing_requests')
    ->condition('id', $id)
    ->execute();
}
