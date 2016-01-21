<?php

/**
 * @file
 * Services integration for the Flag module.
 */

/**
 * Access callback to check a user has access to a flag operation via Services.
 *
 * @param $data
 *   An associative array containing POSTed arguments.
 *   - flag_name: The flag name.
 *   - entity_id: The entity ID that should be flagged.
 *   - uid: Optional. The user ID on behalf to flag the entity.
 *   - action: Optional. The desired method - flag or unflag. Defaults to flag.
 *
 * @return
 *   TRUE if access is allowed.
 */
function flag_service_flag_content_access($data) {

  // Check action is valid.
  if (!in_array($data['action'], array('flag', 'unflag'))) {
    return FALSE;
  }

  $account = NULL;
  if (!empty($data['uid'])) {
    $account = user_load($data['uid']);
  }

  $flag = flag_get_flag($data['flag_name']);

  if (!$flag) {
    // Flag does not exist.
    return services_error(t('There is no flag with the name @flag_name', array('@flag_name' => $data['flag_name'])), 406);
  }

  if (!$flag->access($data['entity_id'], $data['action'], $account)) {
    // User has no permission to use this flag.
    return FALSE;
  }

  return TRUE;
}

/**
 * Service wrapper to flag a entity.
 *
 * @param $data
 *   An associative array containing POSTed arguments.
 *   - flag_name: The flag name.
 *   - entity_id: The entity ID that should be flagged.
 *   - uid: Optional. The user ID on behalf to flag the entity.
 *   - action: Optional. The desired method - flag or unflag. Defaults to flag.
 *   - flagging: Optional. A flagging entity, see flag->flag().
 *
 * @return
 *   TRUE if entity was flagged.
 */
function flag_service_flag_content($data) {
  global $user;
  $account = empty($data['uid']) ? $user : user_load($data['uid']);
  $flag = flag_get_flag($data['flag_name']);
  $skip_permission_check = (boolean) !empty($data['skip_permission_check']) ? $data['skip_permission_check'] : FALSE;
  $flagging = !empty($data['flagging']) ? $data['flagging'] : NULL;
  if ($flagging) {
    $flagging = (object) $flagging;
    $flagging->entity_id = $data['entity_id'];
    $flagging->flag_name = $data['flag_name'];
  }
  if (!$flag) {
    watchdog('services', '<pre>' . print_r(debug_backtrace(), TRUE) . '</pre>');
    // Flag does not exist.
    return services_error(t('There is no flag with the name @flag_name', array('@flag_name' => $data['flag_name'])), 406);
  }
  return $flag->flag($data['action'], $data['entity_id'], $account, $skip_permission_check, $flagging);
}

/**
 * Service wrapper to check if a entity is flagged by a user.
 *
 * @param $data
 *   An associative array containing POSTed arguments.
 *   - flag_name: The flag name.
 *   - entity_id: The entity ID that should be flagged.
 *   - uid: Optional. The user ID on behalf to flag the entity.
 *
 * @return
 *   TRUE if entity was flagged.
 */
function flag_service_is_flagged($data) {
  $flag = flag_get_flag($data['flag_name']);
  if (!$flag) {
    // Flag does not exist.
    return services_error(t('There is no flag with the name @flag_name', array('@flag_name' => $data['flag_name'])), 406);
  }
  return $flag->is_flagged($data['entity_id'], $data['uid']);
}

/**
 * Service wrapper to count a flag on a entity.
 *
 * @param $data
 *   An associative array containing POSTed arguments.
 *   - flag_name: The flag name.
 *   - entity_id: The entity ID that should be flagged.
 *
 * @return
 *   A object contain the number which shows how many users flagged a entity.
 */
function flag_service_flag_countall($data) {
  $query = db_select('flag_counts', 'fc');
  $query->join('flag', 'f', 'f.fid = fc.fid');
  $query->fields('fc', array('count'))->condition('fc.entity_id', $data['entity_id'])->condition('f.name', $data['flag_name']);
  $result = $query->execute()->fetchObject();
  if (!empty($result)){
    return $result;
  } else {
    return services_error(t('There is no flag with node @entity_id', array('@entity_id' => $data['entity_id'])), 406);
  }
}