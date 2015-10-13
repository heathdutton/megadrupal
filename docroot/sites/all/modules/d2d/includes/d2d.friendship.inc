<?php
/**
 * @file
 * Callback functions for establishing friendship.
 */

/**
 * Returns a human understandable text describing the friendship request.
 *
 * @param array $arguments
 *   Array further specifying the request. Ignored in case of friendship
 *   requests.
 *
 * @param int $time_insert
 *   Timestamp the request was inserted.
 *
 * @param int $time_next_send
 *   Timestamp the request will be sent next.
 *
 * @param int $time_invalid
 *   Timestamp from which on the request is invalid.
 *
 * @param int $max_send_trials
 *   Maximal number of trials the request is being sent.
 *
 * @param array $instance
 *   Array describing the instance to which the request is being sent.
 *
 * @param mixed &$persistent_state
 *   Reference to a variable which is used to memorize if an entry for a similar
 *   request is present in the database. $persistent_state is persistant over
 *   several successive calls to d2d_friendship_outgoing_describe() that
 *   operates on the same $instance.
 *
 * @return string
 *   The human understandable text
 */
function d2d_friendship_outgoing_describe($arguments, $time_insert, $time_next_send, $time_invalid, $max_send_trials, $instance, &$persistent_state) {
  $comment = '';
  if (is_null($persistent_state)) {
    $persistent_state = TRUE;
  }
  else {
    $comment = t('(duplicate)');
  }
  $return = '';
  if (!isset($instance['friendship_state'])) {
    $return = t('An error occurred');
  }
  else {
    switch ($instance['friendship_state']) {
      case 0: $return = t('delete friendship / undo friendship request'); break;
      case 1: $return = t('accept / ask friendship'); break;
      case 2: $return = t('this message will not be sent'); break;
      default: $return = t('accept / ask for friendship'); break;
    }
  }
  return $return . ' ' . $comment;
}

/**
 * D2D callback; decides whether to send or not a friendship request.
 *
 * @param array $arguments
 *   Array further specifying the request. Ignored in case of friendship
 *   requests.
 *
 * @param int $time_insert
 *   Timestamp the request was inserted.
 *
 * @param int $time_next_send
 *   Timestamp the request will be sent next.
 *
 * @param int $time_invalid
 *   Timestamp from which on the request is invalid.
 *
 * @param int $max_send_trials
 *   Maximal number of trials the request is being sent.
 *
 * @param array $instance
 *   Array describing the instance to which the request is being sent.
 *
 * @param mixed &$persistent_state
 *   Reference to a variable which is used to memoize if an entry for a similar
 *   request is present in the database. $persistent_state is persistant over
 *   several successive calls to d2d_friendship_outgoing_process() that operates
 *   on the same $instance.
 *
 * @param array &$arguments_send
 *   Ignored for d2d_friendship_outgoing_process().
 *
 * @return bool|null
 *   Returns TRUE if the friendship request should be sent, FALSE if the reqeust
 *   should be ignored and FALSE, if it should be deleted.
 */
function d2d_friendship_outgoing_process($arguments, $time_insert, $time_next_send, $time_invalid, $max_send_trials, $instance, &$persistent_state, &$arguments_send) {

  if (is_null($persistent_state)) {
    $persistent_state = TRUE;
  }
  else {
    // Do not send and delete.
    return NULL;
  }

  if (!isset($instance['friendship_state'])) {
    // Do not send and delete.
    return NULL;
  }

  switch ($instance['friendship_state']) {
    case 0:
      // No friendship, send.
      $arguments_send = array('state' => 'FALSE');
      return TRUE;

    case 1:
      // Friendship, send.
      $arguments_send = array('state' => 'TRUE');
      return TRUE;

    case 2:
      // No friendship, do not send and delete.
      return NULL;

    default:
      $arguments_send = array('state' => 'TRUE');
      return TRUE;
  }
}

/**
 * D2D callback; processes a request to change the friendship state.
 *
 * @param array $exploded_arguments
 *   Associative array specifying the desired new friendship state.
 *   $arguments['state'] is TRUE if and only if the other instance
 *   wants to be friends and FALSE otherwise.
 *
 * @param array &$rpc_info
 *   Associative array containing general information about the request.
 *
 * @param string $encrypted_data
 *   The original encrypted data
 *
 * @param string $signature
 *   The signature attached to the message.
 *
 * @param string &$error_string
 *   A string to be filled with the error, in case there is one.
 *
 * @return bool
 *   TRUE on success
 */
function d2d_friendship_incoming_preprocess($exploded_arguments, &$rpc_info, $encrypted_data, $signature, &$error_string) {
  if (variable_get('d2d_friendship_autoaccept') !== TRUE) {
    // Nothing to do.
    return TRUE;
  }
  if ($exploded_arguments['state'] === 'FALSE' || empty($exploded_arguments['state'])) {
    // Nothing to do.
    return TRUE;
  }
  $instance = $rpc_info['instance'];

  // Are friendship from localhost permitted?
  if (variable_get('d2d_friendship_autoaccept_localhost') === FALSE) {
    if (d2d_address_is_localhost($rpc_info['url'])) {
      $error_string = 'Friendship from localhost are not allowed.';
      return FALSE;
    }
  }

  // Instance previously not found in db, create instance array.
  if ($instance === FALSE) {
    $instance = array(
      'd2d_id' => $rpc_info['d2d_id'],
      'url' => $rpc_info['url'],
      'name' => 'No name',
      'description' => 'No description.',
    );
  }

  // New instance.
  if (!isset($instance['id'])) {
    $instance_id = d2d_api_instance_add($instance);
    if ($instance_id === FALSE) {
      $error_string = 'An error occurred while adding the instance.';
      return FALSE;
    }
    else {
      $instance['id'] = $instance_id;
    }
  }

  // Instance already added to db, but without public key: request one.
  if (!isset($instance['public_key']) || is_null($instance['public_key'])) {
    $pkey = d2d_api_public_key_request($instance);
    if (!empty($pkey)) {
      // Requested public key must be new or belonging to the same instance
      // and previously not selected.
      $public_key_id = d2d_api_public_key_return_id($instance['d2d_id'], $pkey);
      if ($public_key_id === FALSE) {
        $public_key_id = d2d_api_public_key_set($instance['id'], $pkey);
      }

      $key_selected = d2d_api_public_key_select($instance['id'], $public_key_id);

      if (!$key_selected) {
        $error_string = 'Could not select public key for instance.';
        return FALSE;
      }

      $instance['public_key'] = $pkey;
      $instance['public_key_id'] = $public_key_id;

      // Re-evaluate signature_valid
      if (openssl_verify($encrypted_data, $signature, $instance['public_key']) !== 1) {
        $error_string = 'Cannot automatic accept friendship: wrong signature.';
        return FALSE;
      }
      else {
        $rpc_info['signature_valid'] = TRUE;
      }

      // At this point  we have modified the instance (either created from
      // scratch, or added a public key to it).
      $rpc_info['instance'] = $instance;
    }
    else {
      $error_string = 'Could not retrieve the public key.';
      return FALSE;
    }
  }

  if (isset($instance['is_friend']) && $instance['is_friend']) {
    // Already friends: nothing to do.
    return TRUE;
  }

  // Setting friendship state to 2 = 'friendship request sent but not
  // accepted'. This way, when the friendship request will be processed
  // the instance will respond notifying the sender.
  d2d_api_friendship_update($instance['id'], 1);
  $rpc_info['instance']['friendship_state'] = 1;
  return TRUE;
}

/**
 * D2D callback; processes a request to change the friendship state.
 *
 * @param array $arguments
 *   Associative array specifying the desired new friendship state.
 *   $arguments['state'] is TRUE if and only if the other instance
 *   wants to be friends and FALSE otherwise.
 *
 * @param array $rpc_info
 *   Associative array
 *
 * @param bool $live
 *   TRUE if and only if function called as part of a RPC call from the
 *   caller (i.e. we can send a reply with the new friendship state
 *   immediately), FALSE otherwise.
 *
 * @param NULL|array &$return
 *   If not NULL, this value is sent back to the caller
 *
 * @return bool
 *   TRUE on success
 */
function d2d_friendship_incoming_process($arguments, $rpc_info, $live, &$return) {
  $return = NULL;
  if ($arguments['state'] !== 'TRUE' && $arguments['state'] !== 'FALSE') {
    return FALSE;
  }
  if (!$rpc_info['signature_valid']) {
    if ($live) {
      if ($rpc_info['instance']) {
        $old_friendship_state = $rpc_info['instance']['friendship_state'];
        d2d_check_convert_int($old_friendship_state);
      }
      else {
        $old_friendship_state = NULL;
      }
      // Ignore not validated request if it is already
      // represented in the current friendship state.
      if ($arguments['state'] === 'TRUE'
        && ($old_friendship_state === 2 || $old_friendship_state === 3)) {
        return FALSE;
      }
      if ($arguments['state'] === 'FALSE'
        && ($old_friendship_state === 0 || is_null($old_friendship_state))) {
        return FALSE;
      }

      $description = array(
        'url' => $rpc_info['url'],
        'ip' => $rpc_info['ip'],
        'reason' => ($rpc_info['instance'] ? 'public_key' : 'instance'),
      );
      if ($arguments['state'] === 'TRUE') {
        $type = ($old_friendship_state === 1 ? 'unchecked_accept' : 'unchecked_request');
      }
      else {
        if ($old_friendship_state === 1) {
          $type = 'unchecked_reject';
        }
        elseif ($old_friendship_state === 2) {
          $type = 'unchecked_revoke';
        }
        else {
          // $old_friendship_state === 3.
          $type = 'unchecked_remove';
        }
      }
      _d2d_insert_friendship_notification($rpc_info['d2d_id'], $type, $description);
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  $old_friendship_state = $rpc_info['instance']['friendship_state'];

  // Friendship states:
  // 0: no friendship.
  // 1: friendship request sent but not accepted.
  // 2: friendship request received but not accepted.
  // 3: friendship established.
  $friendship_map = array(
    'FALSE' => array(0 => 0, 1 => 0, 2 => 0, 3 => 0),
    'TRUE' => array(0 => 2, 1 => 3, 2 => 2, 3 => 3),
  );
  $types = array(
    'FALSE' => array(0 => '', 1 => 'reject', 2 => 'revoke', 3 => 'remove'),
    'TRUE' => array(0 => 'request', 1 => 'accept', 2 => '', 3 => ''),
  );
  $new_friendship_state = $friendship_map[$arguments['state']][$old_friendship_state];
  if ($new_friendship_state != $old_friendship_state) {
    d2d_api_friendship_update($rpc_info['instance']['id'],
      $new_friendship_state,
      $old_friendship_state,
      $rpc_info['instance']['id'],
      $rpc_info['instance']);
    _d2d_insert_friendship_notification($rpc_info['instance']['d2d_id'],
      $types[$arguments['state']][$old_friendship_state],
      array());
  }
  if ($live) {
    if ($new_friendship_state == 3) {
      $return = array('value' => 'auto_accept');
    }
    else {
      $return = array('value' => '');
    }
  }
  return FALSE;
}

/**
 * Inserts a friendship notification.
 *
 * @param string $d2d_id
 *   The d2d id of an instance
 *
 * @param string $type
 *   The type of the notification
 *
 * @param string $description
 *   The content of the notification
 *
 * @see d2d_api_notification_insert()
 */
function _d2d_insert_friendship_notification($d2d_id, $type, $description) {
  d2d_api_notification_insert('d2d_friendship', $d2d_id,
    d2d_implode(
      array(
        'type' => $type,
        'description' => d2d_implode($description),
      )
    )
  );
}

/**
 * D2D return callback; replies to a friendship request.
 *
 * @param mixed $return
 *   return value from  d2d_friendship_incoming_process.
 *   Accepts the friendship request if set to array auto_accept
 *
 * @param array $arguments
 *   Array of arguments of request
 *
 * @param mixed $arguments_send
 *   Not used
 *
 * @param int $time_insert
 *   Timestamp the request was inserted.
 *
 * @param int $time_next_send
 *   Timestamp the request will be sent next.
 *
 * @param int $time_invalid
 *   Timestamp from which on the request is invalid.
 *
 * @param int $max_send_trials
 *   Maximal number of trials the request is being sent.
 *
 * @param string $error_string
 *   Not used
 *
 * @param array &$instance
 *   Array describing the instance
 *
 * @param mixed &$persistent_state
 *   Represents the state of request. Not used here
 */
function d2d_friendship_outgoing_return($return, $arguments, $arguments_send, $time_insert, $time_next_send, $time_invalid, $max_send_trials, $error_string, &$instance, &$persistent_state) {
  if ($return === FALSE) {
    // TODO: handle it.
  }
  elseif ($return === NULL) {
    // TODO: handle it.
  }
  else {
    $old_friendship_state = $instance['friendship_state'];
    if ($old_friendship_state == 1 && $return['value'] == 'auto_accept') {
      $new_friendship_state = 3;
      $instance['friendship_state'] = $new_friendship_state;
      $instance['is_friend'] = TRUE;
      d2d_api_friendship_update($instance['id'], $new_friendship_state,
        $old_friendship_state, $instance['id'],
        $instance);
      _d2d_insert_friendship_notification($instance['d2d_id'], 'accept', array());
    }
  }
}

/**
 * Creates a human understandable string about the state of friendship requests.
 *
 * @param array $arguments
 *   Array of arguments of request
 *
 * @param array $attributes
 *   Array describing the request
 *
 * @return string
 *   A text describing the state of the friendship request
 */
function d2d_get_friendship_simple($arguments, $attributes) {
  $type = $arguments['type'];
  $instance = d2d_api_instance_get($attributes['d2d_id']);
  if ($instance) {
    $instance_name = check_plain($instance['name']);
    $instance_str = l($instance_name, 'admin/d2d/instances/' . $instance['id'] . '/details');
  }
  else {
    $instance_str = '<i>' . $attributes['d2d_id'] . '</i>';
  }
  if ($type === 'accept') {
    return $instance_str . ' ' . t('has accepted your friendship request.');
  }
  if ($type === 'reject') {
    return $instance_str . ' ' . t('has rejected your friendship request.');
  }
  if ($type === 'request') {
    $return_str = $instance_str . ' ' . t('has sent you a friendship request.');
    if ($instance) {
      $return_str .= ' ' . t('Click !link to reply.', array(
        '!link' => l(t('here'), 'admin/d2d/instances/' . $instance['id'] . '/friendship'),
      ));
    }
    return $return_str;
  }
  if ($type === 'revoke') {
    return $instance_str . ' ' . t('has revoked the friendship request.');
  }
  if ($type === 'remove') {
    return $instance_str . ' ' . t('has terminated the friendship.');
  }
  if ($type === 'unchecked_accept') {
    return $instance_str . ' ' . t('has accepted your friendship request (signature not valid!).');
  }
  if ($type === 'unchecked_reject') {
    return $instance_str . ' ' . t('has rejected your friendship request (signature not valid!).');
  }
  if ($type === 'unchecked_request') {
    return $instance_str . ' ' . t('has sent you a friendship request (signature not valid!).');
  }
  if ($type === 'unchecked_remove') {
    return $instance_str . ' ' . t('has terminated friendship (signature not valid!).');
  }
  if ($type === 'unchecked_revoke') {
    return $instance_str . ' ' . t('has revoked the friendship request (signature not valid!).');
  }
  return 'unknown request';
}

/**
 * Returns a form with the description of the state of a friendship request.
 *
 * @param array $arguments
 *   Array of arguments of the request
 *
 * @param array $attributes
 *   Array describing the request
 *
 * @return array
 *   A form containing the description of the state of a friendship to be built
 */
function d2d_get_friendship_detailed($arguments, $attributes) {
  $build['notifications'] = array(
    '#title' => t('Notification') . ' #' . $attributes['id'],
    '#type' => 'fieldset',
    '#description' => d2d_get_friendship_simple($arguments, $attributes),
  );
  $type = $arguments['type'];
  if (!strncmp($type, 'unchecked_', strlen('unchecked_'))) {
    // $type starts with 'unchecked_'.
    $description = d2d_explode($arguments['description']);
    $build['notifications']['#description'] .= '<br/>';
    if ($description['reason'] === 'public_key') {
      $build['notifications']['#description'] .= t("The reason why the signature could not be checked is that you don't have a public key associated with that instance.");
      $instance = d2d_api_instance_get($attributes['d2d_id']);
      if ($instance) {
        $build['notifications']['#description'] .= ' ' . t('Click !link to associate a public key with that instance.', array('!link' => l(t('here'), 'admin/d2d/instances/' . $instance['id'] . '/public_key')));
      }
    }
    else {
      $build['notifications']['#description'] .= t('The reason why the signature could not be checked is that this instance is not in your database and hence no public key is available. Click !link to proceed with adding this instance.', array('!link' => l(t('here'), 'admin/d2d/prefill')));
      $_SESSION['d2d_add_instance'] = array(
        'url' => $description['url'],
        'd2d_id' => $attributes['d2d_id'],
        'prefill' => FALSE,
      );
    }
  }
  return $build;
}

/**
 * Sets a $_SESSION variable to prefill the data and redirect to 'admin/d2d'.
 */
function d2d_add_instance_prefill() {
  if (isset($_SESSION['d2d_add_instance'])) {
    $_SESSION['d2d_add_instance']['prefill'] = TRUE;
  }
  drupal_goto('admin/d2d');
}
