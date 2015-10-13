<?php

/**
 * @file
 * Page callback file for the team module.
 */

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing teams.
 */

/**
 * Display a themed list of notifications.
 *
 * @param $teaser
 *   Whether the notifications will be displayed in a teaser style.
 *   Default = FALSE.
 */
function pusher_notifications_list($type = 'ajax') {
  global $user;

  // When we list notifications, mark them all as read
  pusher_notifications_mark_read($user);

  // Query for a list of the latest notifications
  $query = db_select('pusher_notifications', 'pn');
  $query->join('pusher_notifications_data', 'pnd', 'pn.nfid = pnd.nfid');

  $result = $query
    ->condition('uid', $user->uid)
    ->fields('pn')
    ->fields('pnd', array('created', 'callback', 'data'))
    ->orderBy('created', 'DESC')
    ->range(0, 12)
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  // Build the list markup
  $markup = '';
  foreach ($result as $record) {
    $markup .= theme('pusher_notifications_notification', array('notification' => $record));
  }

  if ($type == 'ajax') {
    drupal_json_output($markup);
  }
  else {
    return $markup;
  }
}

/**
 * Process variables for pusher_notifications-notification.tpl.php
 */
function template_preprocess_pusher_notifications_notification(&$variables) {
  $notification = $variables['notification'];
  if (function_exists($notification['callback'])) {
    // Return markup from the notification callback
    if ($build = call_user_func($notification['callback'], $notification)) {
      $variables['markup'] = $build['markup'];
      $variables['picture'] = $build['picture'];
    }
  }
}