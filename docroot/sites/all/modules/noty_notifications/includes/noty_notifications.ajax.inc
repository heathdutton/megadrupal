<?php

/**
 * Provide operations and return result as JSON.
 *
 * @global type $user
 */
function noty_notifications_js_notifications($op = 'get', $nid = NULL) {
  global $user;

  if ($user->uid == 0) exit;

  $json = array();
  switch ($op) {
    case 'delete':
      $json['deleted'] = NotyNotification::delete($nid);
      break;
    case 'get':
      // Get pending notifications for current user.
      $noty = new NotyNotifications;
      $json['notifications']['data'] = $noty->getNotificationsJS();
    case 'count':
      $json['notifications']['count'] = NotyNotifications::countNotifications($user->uid);
    default:
      break;
  }

  drupal_json_output(array($json));
  exit;
}
