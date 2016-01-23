<?php
/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Return the version number of the module as a string.
 */
function d2d_get_version() {
  return D2D_VERSION_STRING;
}

/**
 * Returns the options for an XML-RPC request.
 *
 * Used as the third argument when calling xmlrpc().
 *
 * @see xmlrpc()
 */
function d2d_xmlrpc_options() {
  return array(
    'timeout' => variable_get('d2d_timeout', D2D_TIMEOUT_DEFAULT),
  );
}

/**
 * Changes the online state of this D2D-instance.
 *
 * @param bool $online
 *   the desired new online state.
 *
 * @return bool
 *   the new online state.
 */
function d2d_change_online_state($online) {
  return $online ? d2d_go_online() : d2d_go_offline();
}

/**
 * Changes the state of this D2D-instance to offline.
 *
 * @return bool
 *   the new online state.
 */
function d2d_go_offline() {
  variable_set('d2d_online', FALSE);
  return FALSE;
}

/**
 * Changes the state of this D2D-instance to online.
 *
 * @return bool
 *   the new online state.
 */
function d2d_go_online() {
  $public_key = d2d_api_own_public_key_get();
  $private_key = d2d_api_own_private_key_get();
  if (!d2d_check_key_pair($public_key, $private_key)) {
    variable_set('d2d_online', FALSE);
    return FALSE;
  }
  else {
    variable_set('d2d_online', TRUE);
    return TRUE;
  }
}
/**
 * Returns whether this D2D-instance is online.
 */
function d2d_is_online() {
  return variable_get('d2d_online', FALSE);
}

/**
 * Returns time corrected by an offset.
 */
function d2d_get_time() {
  return time() + variable_get('d2d_utc_offset', D2D_UTC_OFFSET_DEFAULT);
}

/**
 * Formats a an enumerable quantity in English language.
 *
 * @param mixed $v
 *   The numeric-like value to format
 *
 * @param string $unit
 *   The name of the unit of measure
 *
 * @param bool $skip
 *   If TRUE, skip the formatting
 *
 * @return string
 *   The formatted string
 */
function d2d_format_unit($v, $unit, $skip = FALSE) {
  if (!$v && $skip) {
    return '';
  }
  else {
    return strval($v) . ' ' . $unit . ($v != 1 ? 's' : '');
  }
}

/**
 * Returns the current time.
 *
 * @param int $t
 *   The timestamp
 *
 * @return string
 *   The formatted timestamp
 */
function d2d_get_absolute_time($t) {
  // TODO: localization.
  return date('F j, Y, g:i a', $t);
}

/**
 * Checks if an URL is valid.
 *
 * @param string $url
 *   The url
 *
 * @return bool
 *   TRUE, if the url is valid
 */
function d2d_check_url($url) {
  if (!is_string($url)) {
    return FALSE;
  }
  if (!preg_match('/^https?:\/\/.+$/', $url)) {
    return FALSE;
  }
  return strlen($url) <= D2D_INSTANCE_URL_MAX_LENGTH;
}

/**
 * Get string describing time relative to current time.
 *
 * @param int $t
 *   The timestamp
 *
 * @return string
 *   The relative time
 *
 * TODO: localization.
 */
function d2d_get_relative_time($t) {
  $diff = $t - d2d_get_time();
  $prefix = '';
  $suffix = '';
  if ($diff > 0) {
    $absdiff = $diff;
    $prefix = 'in ';
  }
  else {
    $absdiff = -$diff;
    $suffix = ' ago';
  }
  $diffs = array(
    gmdate('Y', $absdiff) - 1970,
    gmdate('n', $absdiff) - 1,
    gmdate('j', $absdiff) - 1,
    gmdate('G', $absdiff),
    intval(gmdate('i', $absdiff)),
    intval(gmdate('s', $absdiff)),
    0,
  );
  $relative = '';
  $units = array('year', 'month', 'day', 'hour', 'minute', 'second', '');
  for ($i = 0; $i < 6; $i++) {
    if ($i == 5 || $diffs[$i] > 0) {
      $relative = d2d_format_unit($diffs[$i], $units[$i]);
      $tmp = d2d_format_unit($diffs[$i + 1], $units[$i + 1], TRUE);
      if ($tmp) {
        $relative = "$relative, $tmp";
      }
      break;
    }
  }
  return $prefix . $relative . $suffix;
}

/**
 * Converts a bool to a string.
 *
 * @param bool $b
 *   A boolean value
 *
 * @return string
 *   The converted boolean
 */
function d2d_bool_to_str($b) {
  if (is_null($b)) {
    return t('maybe');
  }
  else {
    return $b ? t('true') : t('false');
  }
}

/**
 * Adds convenient fields to an instance array.
 *
 * Modifies the instance array
 *
 * @param array $instance
 *   The array to be decorated.
 */
function _d2d_decorate_instance_array(&$instance) {
  if (empty($instance)) {
    return;
  }
  $instance['is_friend'] = $instance['friendship_state'] == 3;
  $instance['is_me'] = d2d_api_own_d2d_id_get() == $instance['d2d_id'];
}

/**
 * Performs transformations on list of instances as loaded from the database.
 *
 * @param array $result_set
 *   Array consisting of stdClass objects
 *
 * @param bool $d2d_id
 *   If TRUE, sets the associative keys of each instance to their d2d_id.
 *   Defaults, FALSE (keys are set to their instance id)
 *
 * @return array
 *   The transformed result set
 */
function _d2d_process_result_set_instance($result_set, $d2d_id = FALSE) {
  $instances = array();
  foreach ($result_set as $record) {
    $instances[$d2d_id ? $record->d2d_id : $record->id] = array(
      'id' => $record->id,
      'd2d_id' => $record->d2d_id,
      'name' => $record->name,
      'url' => $record->url,
      'description' => $record->description,
      'time_inserted' => $record->time_inserted,
      'last_alive' => $record->last_alive,
      'public_key' => $record->public_key,
      'friendship_state' => $record->friendship_state,
    );
    _d2d_decorate_instance_array($instances[$d2d_id ? $record->d2d_id : $record->id]);
  }

  return $instances;
}

/**
 * Include instance specified by id in all groups that are marked as default.
 *
 * @param int $id
 *   The id of the instance
 */
function d2d_set_default_group_memberships($id) {
  db_delete('d2d_group_memberships')
    ->condition('instance_id', $id)->execute();
  $groups = d2d_api_group_get(NULL, TRUE);
  if (!empty($groups)) {
    $query = db_insert('d2d_group_memberships')->fields(array('group_id', 'instance_id'));
    foreach ($groups as $group) {
      $query->values(array('group_id' => $group['id'], 'instance_id' => $id));
    }
    $query->execute();
  }
}

/**
 * Checks if a timestamp is not too far off from a specified time.
 *
 * @param int $current_time
 *   The current timestamp
 *
 * @param int $timestamp
 *   Another timestamp to compare
 *
 * @return bool
 *   TRUE, if the timestamp is not beyond the a certain treshold
 *
 * @see d2d_get_time()
 */
function d2d_verify_timestamp($current_time, $timestamp) {
  return $timestamp >= $current_time - variable_get('d2d_delta_0', D2D_DELTA_0_DEFAULT) && $timestamp <= $current_time + variable_get('d2d_delta_1', D2D_DELTA_1_DEFAULT);
}

/**
 * Checks if a nonce has been seen before.
 *
 * @param int $timestamp
 *   The timestamp
 *
 * @param string $nonce
 *   The nonce string
 *
 * @retrn bool
 *   TRUE, if the nonce is new
 *
 * @see d2d_check_nonce()
 */
function d2d_verify_nonce($timestamp, $nonce) {
  try {
    db_insert('d2d_nonces')->fields(array(
      'nonce' => $nonce,
      'timestamp' => $timestamp,
    ))->execute();
  }
  catch (Exception $e) {
    return FALSE;
  }
  return TRUE;
}