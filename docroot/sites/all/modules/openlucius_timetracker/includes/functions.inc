<?php
/**
 * @file
 * This file contains functions for the time-tracker.
 */

/**
 * Function to get the basic time-tracker query.
 *
 * @return \SelectQuery
 *   Returns the basic time-tracker query containing only the entries.
 */
function openlucius_timetracker_basic_query() {
  $query = db_select('openlucius_timetracker_entry', 'e');
  $query->fields('e', array('nid', 'uid', 'start', 'stop', 'message'));
  $query->condition('stop', 0, '!=');
  $query->orderBy('e.start', 'ASC');

  return $query;
}

/**
 * Function to get the advanced time-tracker query.
 *
 * @return \SelectQuery
 *   Returns the advanced time-tracker
 *   containing the various references and labels.
 */
function openlucius_timetracker_advanced_query() {
  $query = openlucius_timetracker_basic_query();
  $query->join('node', 'n', 'n.nid = e.nid');
  $query->join('field_data_field_todo_list_reference', 'ref', 'e.nid = ref.entity_id');
  $query->join('field_data_field_timetracker_type', 'type', 'type.entity_id = ref.field_todo_list_reference_nid');
  $query->join('field_data_field_shared_time_maximum', 'max', 'max.entity_id = ref.entity_id');
  $query->join('field_data_field_todo_label', 's', 's.entity_id = e.nid');

  $query->fields('n', array('title', 'changed'));
  $query->addField('s', 'field_todo_label_tid', 'status');
  $query->addField('type', 'field_timetracker_type_tid', 'type');
  $query->addField('max', 'field_shared_time_maximum_value', 'max');
  $query->addField('type', 'entity_id', 'list');

  return $query;
}

/**
 * Function to start the timer for a user on a given node id.
 *
 * @param int $uid
 *   User id.
 *
 * @param int $nid
 *   Node id.
 */
function _openlucius_timetracker_timer_start($uid, $nid) {

  // Always call the stop before a start.
  _openlucius_timetracker_timer_stop($uid);

  // Get the current time.
  $start = time();

  // Do the start query.
  db_insert('openlucius_timetracker_entry')->fields(array(
    'uid'     => $uid,
    'nid'     => $nid,
    'start'   => $start,
    'stop'    => 0,
    'message' => '',
  ))->execute();
}

/**
 * Function to stop the active time for a user.
 *
 * @param int $uid
 *   User id.
 */
function _openlucius_timetracker_timer_stop($uid) {

  // Get the active timer to stop.
  $time_entry = _openlucius_timetracker_timer_get_active($uid);

  // Only stop if there is an active one.
  if (isset($time_entry->id)) {

    // Update the active time entry and set the stop time to the current time.
    db_update('openlucius_timetracker_entry')->fields(array(
      'stop' => time(),
    ))->condition('id', $time_entry->id, '=')->execute();
  }
}

/**
 * Function to see what users are doing at the moment.
 */
function _openlucius_timetracker_activity() {

  $query = db_select('openlucius_timetracker_entry', 'o')
    ->fields('o', array('uid', 'nid'))
    ->fields('u', array('name'))
    ->fields('n', array('title'));

  // Join on user table for name.
  $query->join('users', 'u', 'o.uid = u.uid');

  // Join on node table for title.
  $query->join('node', 'n', 'o.nid = n.nid');

  $values = $query->condition('start', 0, '>')
                  ->condition('stop', 0, '=')
                  ->execute()
                  ->fetchAll();

  if (!empty($values)) {

    $html = '<ul>';
    foreach ($values as $item) {

      // Shorten username if it's longer than 17 characters.
      $username = $item->name;
      if (strlen($item->name) > 17) {
        if (strpos($username, ' ') !== FALSE) {
          $username = substr($username, 0, strpos($username, ' ')) . ' ...';
        }
        else {
          $username = substr($username, 0, 17) . ' ...';
        }
      }

      // Shorten todo title if it's to long.
      $case_title = $item->title;
      if (strlen($case_title) > 15) {
        $case_title = substr($case_title, 0, 15) . '...';
      }

      $html .= '<li><span class="username">' . $username . '</span><span class="case">' . l($case_title, 'node/' . $item->nid) . '<span></li>';
    }

    $html .= '</ul>';
  }

  return $html;
}

/**
 * Function to get the active timer entry for a user.
 *
 * @param int $uid
 *   User id.
 *
 * @return mixed
 *   The active time entry or FALSE.
 */
function _openlucius_timetracker_timer_get_active($uid) {

  // Find the current active timer for the user.
  $time_entry = db_query("SELECT * FROM {openlucius_timetracker_entry} WHERE uid = :uid AND stop = 0 LIMIT 1", array(":uid" => $uid))->fetchObject();

  if (isset($time_entry->id)) {
    // Return the current active timer.
    return $time_entry;
  }

  // Return false if no active timer.
  return FALSE;
}

/**
 * Function for getting the totals of a single node.
 *
 * @param \stdClass $node
 *   The node object.
 *
 * @return array
 *   An array containing the total time of the user and grand total.
 */
function openlucius_timetracker_get_time_for_node(\stdClass $node) {
  global $user;

  // Load timetracker functions.
  module_load_include('inc', 'openlucius_timetracker', 'includes/page');

  $clocked = array();
  $list    = openlucius_timetracker_todo_reports($node, OPENLUCIUS_TIMETRACKER_RAW);
  foreach ($list['list'] as $list) {
    foreach ($list['cases'] as $case) {
      foreach ($case['log'] as $entry) {
        if ($entry->uid == $user->uid) {
          if (isset($clocked['user'])) {
            $clocked['user'] += $entry->diff;
          }
          else {
            $clocked['user'] = $entry->diff;
          }
        }
        if (isset($clocked['total'])) {
          $clocked['total'] += $entry->diff;
        }
        else {
          $clocked['total'] = $entry->diff;
        }
      }
    }
  }
  return $clocked;
}

/**
 * Function for getting all the node titles.
 *
 * @return array
 *   Array of titles of all the nodes.
 */
function openlucius_timetracker_get_titles() {
  $titles = &drupal_static(__FUNCTION__);

  // Check if the titles where cached if not query them.
  if (!isset($titles)) {

    // Fetch the titles.
    $title_result = db_query("SELECT title, nid FROM {node} n WHERE n.type = 'ol_todo' OR n.type = 'ol_todo_list'");

    foreach ($title_result as $item) {
      $titles[$item->nid] = check_plain($item->title);
    }
  }
  return $titles;
}

/**
 * Function for getting all the users.
 *
 * @return array
 *   Array of users.
 */
function openlucius_timetracker_get_users($anonymous = FALSE) {
  $users = &drupal_static(__FUNCTION__);

  // Check if the users where set.
  if (!isset($users)) {

    // Select all users.
    $query = db_select('users', 'u')->fields('u', array('uid', 'name'));

    // Skip anonymous if default.
    if ($anonymous === FALSE) {
      $query->condition('uid', 0, '<>');
    }

    $users = $query->execute()->fetchAllKeyed(0, 1);
  }

  return $users;
}

/**
 * Function for getting the default query.
 *
 * @return SelectQuery
 *   The altered SelectQuery.
 */
function openlucius_timetracker_get_query() {
  // Default query.
  $query = db_select('openlucius_timetracker_entry', 'e');

  $query->join('node', 'n', 'n.nid = e.nid');
  $query->join('field_data_field_todo_list_reference', 'ref', 'e.nid = ref.entity_id');
  $query->join('field_data_field_timetracker_type', 'type', 'type.entity_id = ref.field_todo_list_reference_nid');
  $query->join('field_data_field_shared_time_maximum', 'max', 'max.entity_id = ref.entity_id');
  $query->join('field_data_field_todo_label', 's', 's.entity_id = e.nid');

  $query->fields('e', array('nid', 'uid', 'start', 'stop', 'message'));
  $query->fields('n', array('title', 'changed'));

  $query->addField('s', 'field_todo_label_tid', 'status');
  $query->addField('type', 'field_timetracker_type_tid', 'type');
  $query->addField('max', 'field_shared_time_maximum_value', 'max');
  $query->addField('type', 'entity_id', 'list');

  $query->condition('stop', 0, '!=');
  $query->groupBy('e.id');
  $query->orderBy('e.start', 'ASC');
  return $query;
}

/**
 * Function for getting certain arguments from the URL.
 *
 * @return array
 *   The array of filters.
 */
function openlucius_timetracker_get_filter() {
  $filter = &drupal_static(__FUNCTION__);

  if (!isset($filter)) {
    $filter = array();

    // Get start time for filter.
    $filter['start'] = isset($_GET['start']) ? filter_xss($_GET['start']) : strtotime("first day of this month midnight");

    // Get end time for filter.
    $filter['end']   = isset($_GET['end']) ? filter_xss($_GET['end']) : strtotime("last day of this month midnight");

    // Get the state.
    if (isset($_GET['state'])) {
      $filter['state'] = explode(',', filter_xss($_GET['state']));
    }

    // Get the active user if set and not zero.
    if (isset($_GET['user']) && $_GET['user'] != 0) {
      $filter['user'] = filter_xss($_GET['user']);
    };
  }

  return $filter;
}

/**
 * Method to stop the active timer.
 */
function openlucius_timetracker_close_active() {
  global $user;

  if (isset($_POST['token']) && drupal_valid_token($_POST['token'])) {
    _openlucius_timetracker_timer_stop($user->uid);
    drupal_json_output(TRUE);
    drupal_exit();
  }

  drupal_json_output(FALSE);
  drupal_exit();
}
