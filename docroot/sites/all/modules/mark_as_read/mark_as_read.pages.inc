<?php

/**
 * @file
 * Page callback file for the mark_as_read module.
 */

/**
 * Page callback for ajax request.
 *
 * Saves the user click info to the database.
 *
 * @return int
 *   Returns access denied info if parameter is invalid.
 */
function mark_as_read_add_attribute() {

  $list_id = $_POST['listId'];
  $attribute_value = $_POST['attributeValue'];
  $token = $_POST['token'];

  if (!$list_id || !$attribute_value || !$token) {
    return MENU_ACCESS_DENIED;
  }

  // Validate Token to prevent from CSRF.
  if (!drupal_valid_token($token, 'user_token')) {
    return MENU_ACCESS_DENIED;
  }

  // Save to the watchdog if its enabled in the admin.
  if (variable_get('mark_as_read_enable_logging', 0)) {
    $list = mark_as_read_db_get_list_by_id($list_id);
    watchdog(
      'mark_as_read',
      'HTML element with attribute name (@attribute_name) and attribute value (@attribute_value) of list @list_name is read',
      array(
        '@attribute_name' => $list->attribute_name,
        '@attribute_value' => $attribute_value,
        '@list_name' => $list->list_name,
      ),
      WATCHDOG_INFO
    );
  }

  mark_as_read_db_save_visited_activity($list_id, $attribute_value);
  // Let other module act after any list item is marked as read.
  module_invoke_all('mark_as_read_marked_read', $list_id, $attribute_value);
}

/**
 * Page Callback.
 *
 * Gets all the lists/MenuList which needs to be tracked based
 * on the page list provided.
 *
 * This list is always based on the current user.
 *
 * @return array()
 *   Array of list object with listId, attributeName, attributeValues(array).
 */
function mark_as_read_get_lists() {
  $lists = mark_as_read_db_get_lists();

  $rows = array();
  foreach ($lists as $list) {
    $rows[$list->list_id] = (array) $list;
  }
  return $rows;
}
