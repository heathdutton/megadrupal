<?php

/**
 * @file
 * Resource callbacks for user_notifications_services module.
 */

/**
 * Callback to get a user notification. Calls $user_notification->seen() before
 * delivering the result.
 *
 * @param $user_notification_id
 *  User notification to return.
 *
 * @return
 *  Loaded user notification entity or a 404 services error if it can't be found.
 */
function _user_notifications_services_resource_retrieve($user_notification_id) {
  $user_notification = user_notifications_load($user_notification_id);

  if ($user_notification) {
    $user_notification->update_state('seen');
    return _user_notifications_services_format_notification($user_notification);
  } else {
    services_error(t('Invalid notification id'), 404);
  }
}

/**
 * Callback to get a list of all user notifications for a given state for the
 * logged in user. Marks each notification as seen when building the result
 * array.
 *
 * @param $states
 *  Array of notification states to pull notifications for. If empty, all states
 *  of notifications will be returned.
 *
 * @param $page
 *  If paginating, the page number being requested. This is multipled by the
 *  pagesize to determine which records to return.
 *
 * @param $limit
 *  How many records should be returned. This is used in combination with $page
 *  to determine which records should be returned. For example, $page = 3 and
 *  $pagesize = 10 would return records 30 - 39.
 *
 * @return
 *  Array with each row referring to a single notification consisting of:
 *    type => type of user notification
 *    user_notification_id => ID of the user notification
 *    state => notification state, ex. unseen, seen
 *    data => array
 *      field_name => value
 */
function _user_notifications_services_resource_index($states = array(), $page = 0, $limit = 10) {
  $user_notification_ids = user_notifications_get_user_notification_ids($states, NULL, $page, $limit);
  $notifications = array();

  if (isset($user_notification_ids['user_notifications'])) {
    foreach ($user_notification_ids['user_notifications'] as $unid => $data) {
      $user_notification = user_notifications_load($unid);
      $user_notification->update_state('seen');
      $notifications[] = _user_notifications_services_format_notification($user_notification);
    }
  }

  return $notifications;
}

/**
 * Callback for updating a notification state.
 *
 * @param $user_notification_id
 *  UserNotification ID to update.
 *
 * @param $state
 *  Machine name for the state that the user notification will be updated to.
 *
 * @return
 *  Boolean TRUE or FALSE depending on whether the update was successful.
 */
function _user_notifications_services_resource_update_state($user_notification_id, $state = 'acknowledged') {
  $user_notification = user_notifications_load($user_notification_id);
  return $user_notification->update_state($state);
}

/**
 * Format a user notification for services. This includes using the entity
 * wrapper to pass back friendlier values.
 *
 * @param $user_notification
 *  A user notification object.
 *
 * @return
 *  The loaded user_notification object with the wrapper unset and field values
 *  replaced with the result of the value() operation on each field.
 */
function _user_notifications_services_format_notification(UserNotification $user_notification) {
  $field_info = field_info_instances('user_notifications', $user_notification->type);
  $fields = array_keys($field_info);
  foreach ($fields as $key => $field_name) {
    $user_notification->$field_name = $user_notification->wrapper->$field_name->value();
  }
  unset($user_notification->wrapper);
  return $user_notification;
}
