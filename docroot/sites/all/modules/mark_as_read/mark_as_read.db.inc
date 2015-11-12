<?php

/**
 * @file
 * This file contains all the DB operations required by other APIs.
 */

/**
 * Delete all the mark_as_read activity for deleted user.
 *
 * Implements hook_user_delete().
 */
function mark_as_read_user_delete($account) {
  mark_as_read_db_delete_by_user($account->uid);
}

/**
 * Returns all the list entry and its direct properties.
 */
function mark_as_read_db_get_all_lists() {
  $query = db_select('mark_as_read', 'lv');
  $query->fields('lv');
  $result = $query->execute();
  return $result->fetchAll();
}

/**
 * Gets the List details By listId.
 *
 * @param int $list_id
 *   Unique List Id.
 *
 * @return object
 *   Single List Object.
 */
function mark_as_read_db_get_list_by_id($list_id) {
  $query = db_select('mark_as_read', 'lv');
  $query->fields('lv')
    ->condition('list_id', $list_id);
  return $query->execute()->fetchObject();
}

/**
 * Get the list by list name. Its must be made sure that only one list.
 *
 * Returns all the fields.
 *
 * @param string $name
 *   Name of the List.
 *
 * @return StdClass
 *   List Object.
 */
function mark_as_read_db_get_list_by_name($name) {
  $query = db_select('mark_as_read', 'lv');
  $query->fields('lv')
  ->condition('list_name', $name);
  return $query->execute()->fetchObject();
}

/**
 * Get all the lists with user activity with read/unread.
 *
 * It does not return the UID.
 */
function mark_as_read_db_get_lists() {
  global $user;
  $query = db_select('mark_as_read', 'lv');
  $query->fields('lv', array('list_id', 'css_selector', 'attribute_name'))
  ->fields('lva', array('attribute_values'))
  ->leftJoin('mark_as_read_activity', 'lva', "lv.list_id = lva.list_id AND lva.uid = {$user->uid}");

  $result = $query->execute();

  $rows = array();
  while ($record = $result->fetchObject()) {
    $record->attribute_values = unserialize($record->attribute_values);
    $rows[] = $record;
  }
  return $rows;
}

/**
 * Update the List details to the database.
 */
function mark_as_read_update_list_details($list_id, $name, $css_selector, $attribute_name) {

  return db_update('mark_as_read')
  ->fields(array(
    'list_name' => $name,
    'css_selector' => $css_selector,
    'attribute_name' => $attribute_name,
  ))
  ->condition('list_id', $list_id)
  ->execute();
}

/**
 * Update the list details by name.
 *
 * Its updating via list name as name is the unique
 * identifier for the features export.
 */
function mark_as_read_update_list_details_by_name($name, $css_selector, $attribute_name) {
  return db_update('mark_as_read')
  ->fields(array(
    'list_name' => $name,
    'css_selector' => $css_selector,
    'attribute_name' => $attribute_name,
  ))
  ->condition('list_name', $name)
  ->execute();
}

/**
 * Inserts the List details to the database.
 */
function mark_as_read_insert_list_details($name, $css_selector, $attribute_name) {

  return db_insert('mark_as_read')
  ->fields(array(
      'list_name' => $name,
      'css_selector' => $css_selector,
      'attribute_name' => $attribute_name,
  ))
  ->execute();
}

/**
 * Get all the lists details and attribute values of current user.
 *
 * @param int $list_id
 *   Individual ListId.
 */
function mark_as_read_db_get_user_activity($list_id) {
  global $user;
  $result = db_select('mark_as_read_activity', 'lva')
    ->fields('lva')
    ->condition('list_id', $list_id)
    ->condition('uid', $user->uid)
    ->execute();
  return $result->fetchAll();
}

/**
 * First try to update, if there is no row , then it tries to insert the data.
 *
 * @param int $list_id
 *   Unique List Id.
 * @param string $attribute_value
 *   Attribute value of the clicked element.
 */
function mark_as_read_db_save_visited_activity($list_id, $attribute_value) {
  try {
    mark_as_read_db_update_visited_activity($list_id, $attribute_value);
  }
  catch (Exception $e) {
    mark_as_read_db_insert_visited_activity($list_id, $attribute_value);
  }
}

/**
 * Update the user activity.
 *
 * First , get the serialized list of attribute values and,
 * then append the new attribute value and then again serialized and updates.
 *
 * @throws LogicException
 *   If there is no row in the activity table then we can't update.
 */
function mark_as_read_db_update_visited_activity($list_id, $attribute_value) {
  global $user;

  $list_activity = mark_as_read_db_get_list_activity_of_current_user($list_id);

  // We can't update if there is no row on the table.
  if (!$list_activity) {
    throw new LogicException(t("There is no user activity for the specified list. Hence, update failed"));
  }

  $attribute_values = unserialize($list_activity->attribute_values);

  // Not need to insert attribute value if it already exists.
  if (in_array($attribute_value, $attribute_values)) {
    return;
  }
  // As this data directly comes from the URL, so filter it.
  $attribute_values[] = $attribute_value;
  return db_update('mark_as_read_activity')
  ->fields(array(
      'attribute_values' => serialize($attribute_values),
  ))
  ->condition('list_id', $list_id)
  ->condition('uid', $user->uid)
  ->execute();
}

/**
 * Insert the user activity.
 */
function mark_as_read_db_insert_visited_activity($list_id, $attribute_value) {
  global $user;
  return db_insert('mark_as_read_activity')
  ->fields(array(
    'list_id' => $list_id,
    'uid' => $user->uid,
    'attribute_values' => serialize(array($attribute_value)),
  ))
  ->execute();
}

/**
 * Get the List row based on the list Id and user Id.
 *
 * @param int $list_id
 *   Individual ListId.
 */
function mark_as_read_db_get_list_activity_of_current_user($list_id) {
  global $user;
  $result = db_select('mark_as_read_activity', 'lva')
    ->fields('lva')
    ->condition('list_id', $list_id)
    ->condition('uid', $user->uid)
    ->execute()
    ->fetchObject();
  return $result;
}

/**
 * Delete list details and all the activity for that particular list.
 *
 * @param int $list_id
 *   Unique List Id.
 */
function mark_as_read_db_delete_list($list_id) {

  mark_as_read_db_delete_list_activity($list_id);
  return db_delete('mark_as_read')
  ->condition('list_id', $list_id)
  ->execute();
}

/**
 * Delete List Read/Unread Activity.
 *
 * @param int $list_id
 *   Unique List Id.
 */
function mark_as_read_db_delete_list_activity($list_id) {
  return db_delete('mark_as_read_activity')
  ->condition('list_id', $list_id)
  ->execute();
}

/**
 * Delete the record by user Id.
 *
 * @param int $uid
 *   Drupal UserId.
 */
function mark_as_read_db_delete_by_user($uid) {
  return db_delete('mark_as_read_activity')
  ->condition('uid', $uid)
  ->execute();
}
