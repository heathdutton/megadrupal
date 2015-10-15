<?php
/**
 * @file
 * Notifications if instance is offline etc.,
 * a few functions to list notifications in a table, show details,
 * mark them as read, delete them etc.
 */

/**
 * Adds a new entry into the notification system.
 *
 * Notifications are about instance being offline etc.
 *
 * @param array $exclude
 *   an array specifying which notifications are not shown
 */
function d2d_notify($exclude = array()) {
  if (!in_array('offline', $exclude) && !d2d_is_online()) {
    drupal_set_message(t('D2D is currently in offline mode. In offline mode, no signed / encrypted queries will be sent and no queries are being answered. Click !link to change the settings.', array('!link' => l(t('here'), 'admin/d2d/settings/advanced'))), 'warning', FALSE);
  }
  if (!in_array('public_key', $exclude)) {
    $message = t('At least one instance in the database has no public key. Click !link to show an overview of all instances.', array('!link' => l(t('here'), 'admin/d2d')));
    if (d2d_api_instance_without_public_key_exists()) {
      drupal_set_message($message, 'warning', FALSE);
    }
    else {
      if (isset($_SESSION['messages']['warning'])) {
        $key = array_search($message, $_SESSION['messages']['warning']);
        if ($key !== FALSE) {
          unset($_SESSION['messages']['warning'][$key]);
          if (empty($_SESSION['messages']['warning'])) {
            unset($_SESSION['messages']['warning']);
          }
        }
      }
    }
  }
  if (!in_array('notifications', $exclude)) {
    $n_unread = d2d_api_notification_count_unread();
    if ($n_unread > 0) {
      if ($n_unread == 1) {
        drupal_set_message(t('You have @count unread notification. Click !link to read it.', array(
          '@count' => $n_unread,
          '!link' => l(t('here'), 'admin/d2d/notifications'),
        )), 'status', FALSE);
      }
      else {
        drupal_set_message(t('You have @count unread notifications. Click !link to read them.', array(
          '@count' => $n_unread,
          '!link' => l(t('here'), 'admin/d2d/notifications'),
        )), 'status', FALSE);
      }
    }
  }
}

/**
 * Implements hook_d2d_handle_notification().
 */
function d2d_d2d_handle_notification() {
  $types['d2d_message'] = array(
    'arguments' => array('subject' => 'is_string', 'text' => 'is_string'),
    'callback_simple' => 'd2d_get_message_simple',
    'callback_detailed' => 'd2d_get_message_detailed',
    'name' => t('system'),
  );
  $types['d2d_friendship'] = array(
    'arguments' => array('type' => 'is_string', 'description' => 'is_string'),
    'callback_simple' => 'd2d_get_friendship_simple',
    'callback_detailed' => 'd2d_get_friendship_detailed',
    'name' => t('friendship'),
  );
  return $types;
}

/**
 * Callback for getting a short description of a general message.
 *
 * @param array $arguments
 *   The array of arguments of the request
 *
 * @param array $attributes
 *   Not used
 *
 * @return string
 *   The short description of a general message
 */
function d2d_get_message_simple($arguments, $attributes) {
  return $arguments['subject'];
}

/**
 * Callback for getting a detailed description of a message.
 *
 * @param array $arguments
 *   The array of arguments of the request
 *
 * @param array $attributes
 *   Array describing the notification
 *
 * @return array
 *   A form ready to be built with the description of a notification
 */
function d2d_get_message_detailed($arguments, $attributes) {
  $build['notifications'] = array(
    '#title' => t('Notification') . ' #' . $attributes['id'],
    '#type' => 'fieldset',
    '#description' => $arguments['subject'] . '<p>' . $arguments['text'] . '</p>',
  );
  return $build;
}

/**
 * Builts a table with notifications of type specified in $types.
 *
 * @param array $types
 *   If not empty, only of notifications of the type contained in the array
 *   will be displayed
 *
 * @return array $table
 *   The notification table
 *
 * @see d2d_api_notification_get_by_type()
 */
function d2d_built_notification_table($types) {
  $build['notifications'] = array(
    '#title' => t('Notifications'),
    '#type' => 'fieldset',
  );
  $hooks = module_invoke_all('d2d_handle_notification');
  $rows = array();
  $notifications = d2d_api_notification_get_by_type($types);
  foreach ($notifications as $notification) {
    if (!array_key_exists($notification['type'], $hooks)) {
      continue;
    }
    $hook = $hooks[$notification['type']];
    $argument_checks = (array_key_exists('arguments', $hook) ? $hook['arguments'] : array());
    $arguments = d2d_explode($notification['content']);
    if (!d2d_check_array($arguments, $argument_checks)) {
      continue;
    }
    $attributes = array(
      'id' => $notification['id'],
      'd2d_id' => $notification['d2d_id'],
      'timestamp' => $notification['timestamp'],
    );
    $type = $hook['name'];
    $message = call_user_func($hook['callback_simple'], $arguments, $attributes);
    if ($notification['seen']) {
      $read_text = t('mark as unread');
      $read_link = 'admin/d2d/notifications/' . $notification['id'] . '/mark_unread';
    }
    else {
      $read_text = t('mark as read');
      $read_link = 'admin/d2d/notifications/' . $notification['id'] . '/mark_read';
    }
    $row = array(
      $notification['id'],
      $type,
      $message,
      d2d_get_absolute_time($notification['timestamp']),
      l(t('details'), 'admin/d2d/notifications/' . $notification['id'] . '/details'),
      l($read_text, $read_link),
      l(t('delete'), 'admin/d2d/notifications/' . $notification['id'] . '/delete'),
    );
    if (!$notification['seen']) {
      foreach ($row as &$cell) {
        $cell = '<strong>' . $cell . '</strong>';
      }
    }
    $rows['id' . $notification['id']] = $row;
  }
  $table = array(
    '#theme' => 'table',
    '#header' => array(
      'ID',
      'Type',
      'Message',
      'Time',
      array('data' => t('Operations'), 'colspan' => 3),
    ),
    '#rows' => $rows,
    '#empty' => t('No notifications found.'),
  );
  return $table;
}

/**
 * Shows the notifications tab.
 */
function d2d_show_notifications() {
  d2d_notify(array('notifications'));
  $build = array();
  $build['notifications'] = array(
    '#title' => t('Notifications'),
    '#type' => 'fieldset',
  );
  $build['notifications'][] = d2d_built_notification_table(NULL);
  return $build;
}

/**
 * Shows details for the notification with id $id.
 *
 * @param int $id
 *   The id of a notification
 *
 * @return array $build
 *   The array containing the form with the details of a notification
 */
function d2d_show_notification_details($id) {
  $error_flag = FALSE;
  if (!d2d_check_convert_int($id)) {
    $error_flag = TRUE;
  }
  else {
    $notification = d2d_api_notification_get($id);
    if (is_null($notification)) {
      $error_flag = TRUE;
    }
  }
  if ($error_flag) {
    drupal_set_message(t('Notification not found'), 'error');
    drupal_goto('admin/d2d/notifications');
  }
  else {
    if (!$notification['seen']) {
      d2d_api_notification_change_read($id, TRUE);
    }
    $hooks = module_invoke_all('d2d_handle_notification');
    $hook = $hooks[$notification['type']];
    $argument_checks = (array_key_exists('arguments', $hook) ? $hook['arguments'] : array());
    $arguments = d2d_explode($notification['content']);
    if (!d2d_check_array($arguments, $argument_checks)) {
      drupal_set_message(t('Notification erroneous'), 'error');
      drupal_goto('admin/d2d/notifications');
    }
    $attributes = array(
      'id' => $notification['id'],
      'd2d_id' => $notification['d2d_id'],
      'timestamp' => $notification['timestamp'],
    );
    $build = call_user_func($hook['callback_detailed'], $arguments, $attributes);
    return $build;
  }
}

/**
 * Menu callback; deletes a notification and redirects to the overview.
 *
 * @param int $id
 *   The id of the notification
 */
function d2d_notification_delete_submit($form, &$form_state, $id) {
  if (d2d_check_convert_int($id)) {
    d2d_api_notification_delete($id);
  }
  drupal_goto('admin/d2d/notifications');
}

/**
 * Menu callback; marks a notification as read and redirects to the overview.
 *
 * @param int $id
 *   The id of the notification
 */
function d2d_notification_mark_read_submit($form, &$form_state, $id) {
  if (d2d_check_convert_int($id)) {
    d2d_api_notification_change_read($id, TRUE);
  }
  drupal_goto('admin/d2d/notifications');
}

/**
 * Menu callback; marks a notification as unread and redirects to the overview.
 *
 * @param int $id
 *   The id of the notification
 */
function d2d_notification_mark_unread_submit($form, &$form_state, $id) {
  if (d2d_check_convert_int($id)) {
    d2d_api_notification_change_read($id, FALSE);
  }
  drupal_goto('admin/d2d/notifications');
}
