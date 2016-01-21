<?php

/**
 * @file
 * Contains DB CRUD methods used by the Cloud Metering module
 * Works with Cloud Metering module
 *
 * Copyright (c) 2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Get cloud instance cost in given period of time
 * @param $start datetime Start date
 * @param $end datetime End Date
 * @param $cloud_context string The cloud name
 * @param $instance_id string The instance id
 * @return array The result of the database query
 */
function _cloud_metering_instance_cost_data($start, $end, $cloud_context = '', $instance_id = '') {

  if ($start && $end) {
    if ($end) {
      $now = date('c');
      $end = (strtotime($end) < time()) ? $end : $now;
    }

    $start_search     = "if (launch_time < '" . $start . "', '" . $start . "', launch_time) ";
    $end_search       = "if (instance_state_name != 'terminated', '" . $end . "', if (terminated_date < '" . $end . "' and terminated_date > '" . $start . "', terminated_date, '" . $end . "')) ";

    $search_where_sub = ' and launch_time < ' . $end_search . " and (terminated_date > '" . $start . "' or terminated_date = '0000-00-00 00:00:00' or terminated_date IS null) and instance_state_name != 'pending' ";
  }


  if ($cloud_context) {
    $search_where = " and a.cloud_type = '" . $cloud_context . "'";
  }
  else {
    $search_where = '';
  }

  if ($instance_id) {
    $search_instance = " and a.instance_id = '" . $instance_id . "'";
  }
  else {
    $search_instance = '';
  }

  //Construct a query for getting the Clound Instance Pricing details with total hours in running
  $query = 'SELECT a.instance_id, a.instance_type, a.instance_state_name, a.cloud_type, a.launch_time, b.linux_or_unix_usage, b.windows_usage ' . 
     'FROM  {' . CLOUD_BILLING_INSTANCES_DETAILS_TABLE . '} a LEFT JOIN {' . CLOUD_PRICING_INSTANCES_TABLE . '} b ON a.instance_type = b.instance_type where a.cloud_type = b.cloud_type ' . $search_instance . $search_where;
  $query_result = cloud_get_db_results($query);

  return $query_result[0];
}

/**
 *  Check if a notification has been sent against an instance
 */
function cloud_metering_notification_sent($type, $cloud_context = '', $instance_id = '', $uid = 0, $gid = 0, $interval = CLOUD_METERING_MAIL_INTERVAL) {
  $query = db_select(CLOUD_METERING_MAILS_TABLE);
  
  if (!empty($cloud_context) && !empty($instance_id)) {
    $query
      ->condition('cloud_name', $cloud_context)
      ->condition('instance_id', $instance_id);
  }
  elseif ($uid) {
    $query->condition('uid', $uid);
  }
  elseif ($gid) {
    $query->condition('gid', $gid);
  }

  // add in time limit
  $count = $query
    ->condition('type', $type)
    ->condition('timestamp', time() - $interval, '>')
    ->countQuery()->execute()->fetchField();
  return $count;
}

/**
 * Save notification mail sending info
 */
function cloud_metering_save_notification_info($type, $cloud_context = '', $instance_id = '', $uid = 0, $gid = 0) {
  $timestamp = time();
  $token = drupal_get_token($cloud_context . $instance_id . $timestamp);


  $nid = db_insert(CLOUD_METERING_MAILS_TABLE) 
    ->fields(array(
    'cloud_name' => $cloud_context,
    'instance_id' => $instance_id,
    'uid' => $uid,
    'gid' => $gid,
    'timestamp' => $timestamp,
    'token' => $token,
    'token_used' => 0,
    'type' => $type,
  ))
    ->execute();
}

/**
 *  Delete metering data related to specific cloud
 */
function _cloud_metering_delete_cloud_data($cloud_context) {

  $num_deleted = db_delete(CLOUD_METERING_INSTANCES_BUDGET_TABLE)
    ->condition('cloud_context', $cloud_context)
    ->execute();
}

/**
 * Delete an instance_id from the 
 * @param unknown_type $cloud_context
 * @param unknown_type $instance_id
 */
function _cloud_metering_instance_budget_delete($cloud_context, $instance_id) {
  db_delete(CLOUD_METERING_INSTANCES_BUDGET_TABLE)
    ->condition('instance_id', $instance_id)
    ->condition('cloud_context', $cloud_context)
    ->execute();
}

/**
 *  Update instance budget data
 *  @param string cloud_context
 *  @param string instance_id
 *  @param array data
 *                array containing the budget and the threshold
 */
function _cloud_metering_instance_budget_save($cloud_context, $instance_id, $data) {
  $budget    = (isset($data['budget']))    ? $data['budget'] : NULL;
  $threshold = (isset($data['threshold'])) ? $data['threshold'] : NULL;
  $terminate = (isset($data['terminate'])) ? $data['terminate'] : 0;  // defaults to false
  
  if (is_null($budget) && is_null($threshold)) {
    return;
  }
  
  $existing_data = _cloud_metering_get_instance_budget($cloud_context, $instance_id);
  if ($existing_data != FALSE) {
    $fields = array();
    if (!is_null($budget)) {
      $fields['initial_budget'] = $budget;
    }
    if (!is_null($threshold)) {
      $fields['cost_threshold'] = $threshold;
    }
    $fields['terminate'] = $terminate;
    
    db_update(CLOUD_METERING_INSTANCES_BUDGET_TABLE)
      ->fields($fields)
      ->condition('cloud_context', $cloud_context, '=')
      ->condition('instance_id', $instance_id, '=')
      ->execute();
  }
  else {
    $budget    = is_null($budget) ? 0 : $budget;
    $threshold = is_null($threshold) ? 0 : $threshold;

    db_insert(CLOUD_METERING_INSTANCES_BUDGET_TABLE) 
      ->fields(array(
        'cloud_context' => $cloud_context,
        'instance_id' => $instance_id,
        'initial_budget' => $budget,
        'cost_threshold' => $threshold,
      ))
      ->execute();
  }
  drupal_set_message(t('The instance budget settings have been saved.'));
}

/**
 *  Get instance budget data
 *  @param string cloud_context
 *  @param string instance_id
 *  @return array data
 *    array containing instance_id, budget and threshold
 */
function _cloud_metering_get_instance_budget($cloud_context, $instance_id) {
  return db_select(CLOUD_METERING_INSTANCES_BUDGET_TABLE, 'c')
    ->fields('c', array('instance_id', 'initial_budget', 'cost_threshold', 'terminate'))
    ->condition('cloud_context', $cloud_context)
    ->condition('instance_id', $instance_id)
    ->execute()->fetchAssoc();
}

/**
 *  Get template budget data
 *  @param int template_id
 *  @return array
 *    array containing template_id, budget and threshold
 */
function _cloud_metering_get_template_budget($template_id) {
  return db_select(CLOUD_METERING_TEMPLATES_BUDGET_TABLE, 'c')
    ->fields('c')
    ->condition('template_id', $template_id)
    ->execute()->fetchAssoc();
}

/**
 * Delete template budget given a template_id
 */
function _cloud_metering_delete_template_budget($template_id) {
  db_delete(CLOUD_METERING_TEMPLATES_BUDGET_TABLE)
    ->condition('template_id', $template_id)
    ->execute();
}

/**
 *  Update template budget
 *  @param int template_id
 *  @param array data
 *    Array containing the budget and the threshold
 */
function _cloud_metering_template_budget_save($template_id, $data) {
  // User must have privilege to edit server templates
  $budget    = (isset($data['budget'])) ? $data['budget'] : NULL;
  $threshold = (isset($data['threshold'])) ? $data['threshold'] : NULL;

  if (is_null($budget) && is_null($threshold)) {
    drupal_set_message(t('No Budget or Threshold passed.  Cannot save budget', 'error'));
    return;
  }

  $existing_data = _cloud_metering_get_template_budget($template_id);

  if ($existing_data) {
    // update an existing template
    $query_update = db_update(CLOUD_METERING_TEMPLATES_BUDGET_TABLE);
    $fields = array();
    if (isset($budget)) {
      $fields['initial_budget'] = $budget;
    }
    if (isset($threshold)) {
      $fields['cost_threshold'] = $threshold;
    }

    $query_update->fields($fields);
    $query_update
      ->condition('template_id', $template_id, '=')
      ->execute();
  }
  else {
    $budget    = is_null($budget) ? 0 : $budget;
    $threshold = is_null($threshold) ? 0 : $threshold;

    $result = db_insert(CLOUD_METERING_TEMPLATES_BUDGET_TABLE) 
      ->fields(array(
      'template_id' => $template_id,
      'initial_budget' => $budget,
      'cost_threshold' => $threshold,
      ))
      ->execute();
  }
  drupal_set_message(t('The template budget settings have been saved.'));
}

/**
 *  Get server template data from db
 */
function _cloud_metering_get_template_data($template_id) {
  return db_select(CLOUD_SERVER_TEMPLATES_TABLE, 'c')
    ->fields('c')
    ->condition('template_id', $template_id)
    ->execute()
    ->fetchAssoc();
}

/**
 * Add group budget into the database
 */
function _cloud_metering_add_group_budget($group_id, $group_threshold, $group_budget, $user_data) {
  return db_insert(CLOUD_METERING_BUDGET_GROUP_TABLE)
    ->fields(array(
      'user_data' => $user_data,
      'grid' => $group_id,
      'threshold' => $group_threshold,
      'group_budget' => $group_budget,
    ))
   ->execute();
}

/**
 * Add user budget into the database
 */
function _cloud_metering_add_user_budget($uid, $group_id, $group_threshold, $user_budget) {
  return db_insert(CLOUD_METERING_USER_BUDGET_TABLE)
    ->fields(array(
      'id' => NULL,
      'uid' => $uid,
      'grid' => $group_id,
      'threshold' => $group_threshold,
      'budget' => $user_budget,
    ))
    ->execute();
}

/**
 * Update Group budget
 */
function _cloud_metering_update_group_budget($group_id, $group_threshold, $group_budget, $user_data) {
  return db_update(CLOUD_METERING_BUDGET_GROUP_TABLE)
    ->fields(array(
      'user_data' => $user_data,
      'threshold' => $group_threshold,
      'group_budget' => $group_budget,
    ))
    ->condition('grid', $group_id, '=')
    ->execute();
}

/**
 * Update User Budget
 */
function _cloud_metering_update_user_budget($uid, $group_id, $user_budget) {
  return db_update(CLOUD_METERING_USER_BUDGET_TABLE)
    ->fields(array(
      'budget' => $user_budget,
    ))
    ->condition('uid', $uid, '=')
    ->condition('grid', $group_id, '=')
    ->execute();
}


/**
 *  Get user groups
 *  @param Array gids
 *    Optional array of group-ids to return
 *  @return Array
 *    The database rows corresponding to the groups
 */
function _cloud_metering_get_groups($gids = array()) {

  $return = array();
  $query = db_select(CLOUD_METERING_BUDGET_GROUP_TABLE, 'c')
    ->fields('c');
  if (count($gids)) {
    $query->condition('c.gid', $gids, 'IN');
  }
  $results = $query->execute();
  while ($record = $results->fetchAssoc()) {
    $return[] = $record;
  } 
  return $return;
}

/**
 *  Get group admins
 *  @param Integer gid The group-id
 *  @return Array The group admin(s)
 */
function _cloud_metering_get_group_admins($gid) {
  $result = db_select(CLOUD_METERING_BUDGET_GROUP_TABLE, 'c')
    ->fields('c', array('user_data'))
    ->condition('gid', $gid)
    ->execute()
    ->fetchField();

  if ($result !== FALSE) {
    $result = unserialize($result);
    if (is_array($result) && sizeof($result) > 0) {
      $return = array();
      foreach ($result as $uid => $value) {
        if ($value > 0) {
          $return[] = $uid;
        }
      }
      return $return;
    }
  }
  return array(1); // Return the super-user in case there is no group admin(s)
}

/**
 * Return roles in the database
 */
function _cloud_metering_get_group_details($rid = '') {
  $query = db_select('role', 'r')
    ->fields('r', array('rid', 'name'));

  // $rid is passed, return only that role  
  if (!empty($rid)) {
    $query->condition('r.rid', $rid);
  }
  else {
    $query->condition('r.rid', 2, '>');
  }
  return $query->execute()->fetchAllAssoc('rid', PDO::FETCH_ASSOC);
}

/**
 * Get Group Budget.  If gid or rid are passed, add those as
 * conditions to the query
 */
function _cloud_metering_get_group_budget($gid = '', $rid = '') {
  $key = 'gid';
  $query = db_select(CLOUD_METERING_BUDGET_GROUP_TABLE, 'cm')
    ->fields('cm');
  if (!empty($gid)) {
    $query->condition('cm.gid', $gid);
  }
  if (!empty($rid)) {
    $query->condition('cm.grid', $rid);
    $key = 'grid';
  }
  return $query->execute()->fetchAllAssoc($key, PDO::FETCH_ASSOC);
}


/**
 * Get User Allocated Budget
 */
function _cloud_metering_get_user_budget($uid) {
  $result = db_select(CLOUD_METERING_USER_BUDGET_TABLE, 'cm')
    ->fields('cm', array('budget'))
    ->condition('cm.uid', $uid)
    ->execute()
    ->fetchField();
  
  if ($result != FALSE) {
    return $result;
  }
  else {
    return 0;
  }
}

/**
 * Get all Drupal Users where uid > 1
 */
function _cloud_metering_get_users_list_by_role($rid) {
  $query = db_select('users', 'u')
    ->fields('u', array('uid', 'mail', 'name'))
    ->fields('ur', array('rid'))
    ->condition('ur.rid', $rid)
    ->condition('u.uid', 1, '>');
  
  $query->innerJoin('users_roles', 'ur', 'u.uid=ur.uid');
  return $query->execute()->fetchAllAssoc('uid', PDO::FETCH_ASSOC);
}
