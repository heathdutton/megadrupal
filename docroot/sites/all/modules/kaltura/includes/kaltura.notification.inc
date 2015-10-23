<?php
/**
 * @file
 * Contains functions for handling all notifications from kaltura.
 */

/**
 * This is the callback function for the kaltura/notification_handler URL.
 *
 * This function uses the KalturaNotificationClient class to normalize the
 * received notifications.
 * The mentioned class also validates the notification signature, to prevent
 * malicious data injection.
 *
 * Each notification is save in the notifications table, so in case of duplicate
 * notification only the first notification will be handled.
 *
 * This function also invokes the hook_notification_handler so other modules
 * that want to act upon notification is received will be able to do so.
 */
function kaltura_notification_handler() {
  $admin_secret = variable_get('kaltura_admin_secret');
  // @todo Validate each needed parameter from $_POST. This is security issue.
  $params = $_POST;
  watchdog('kaltura NH', print_r($params, TRUE));
  unset($params['q']);
  $noti = new KalturaNotificationClient($params, $admin_secret);
  if ($noti->valid_signature === NULL) {
    watchdog('kaltura', 'notification params empty');
    return FALSE;
  }
  elseif ($noti->valid_signature === FALSE) {
    watchdog('kaltura', 'notification signature not valid');
    return FALSE;
  }
  else {
    watchdog('kaltura', 'hooray!!! notification signature valid');
  }
  // TODO: do we really need this echo?
  echo 'OK';
  foreach ($noti->data as $notification_data) {
    $times = kaltura_notification_received($notification_data['notification_id']);
    if (empty($times) || $notification_data['notification_type'] == 'test') {
      kaltura_notification_save($notification_data['notification_id'], $notification_data);
      kaltura_forward_notification($notification_data);
      kaltura_invoke('notification_handler', $notification_data);
    }
    else {
      watchdog('notification', 'This notification (' . $notification_data['notification_id'] . ') was already received');
    }
  }
}

/**
 * This function saves the received notification to the DB.
 *
 * @param int $notification_id
 * @param array $notification_data
 */
function kaltura_notification_save($notification_id, $notification_data) {
  $notify['notification_id'] = $notification_id;
  $notify['data'] = serialize($notification_data);
  $notify['received_at'] = time();

  $entity = entity_create('kaltura_notification', $notify);
  entity_save('kaltura_notification', $entity);
}

/**
 * Checks if the received notification was already handled to avoid duplicates.
 *
 * @param int $notification_id
 *
 * @return bool
 */
function kaltura_notification_received($notification_id) {
  return (bool) entity_load_single('kaltura_notification', $notification_id);
}

/**
 * Checks the notification type and forwards the notification to the appropriate
 * function.
 *
 * Some notification types, as described in kaltura API documentation, are not
 * yet handled here.
 */
function kaltura_forward_notification($noti) {
  watchdog('kaltura notification handler', print_r($noti, TRUE));

  switch ($noti['notification_type']) {
    case 'entry_add':
      kaltura_notify_entry_add('entry', $noti);
      break;

    case 'kshow_add':
      kaltura_notify_entry_add('mix', $noti);
      break;

    case 'kshow_update_info':
      watchdog('KNH', 'notification of type kshow_update_info received<br />' . serialize($noti));
      break;

    case 'entry_update_thumbnail':
      kaltura_update_entry_thumbnail($noti);
      break;

    case 'entry_update':
      kaltura_notify_update($noti);
      break;

    case 'entry_delete':
      kaltura_notify_delete($noti);
      break;

    case 'entry_block':
      kaltura_notify_blocked($noti);
      break;

    case 'test':
      kaltura_test_notification_received();
      break;
  }
}

/**
 * Helper function to set the notification_status variable to 1.
 *
 * Should only be called when test notification received.
 */
function kaltura_test_notification_received() {
  variable_set('kaltura_notification_status', 1);
}

/**
 * Helper function to update the thumbnail url of an entry.
 *
 * @param $notification
 */
function kaltura_update_entry_thumbnail($notification) {
  $entity = kaltura_entry_load($notification['entry_id']);
  $entity->kaltura_thumbnail_url = $notification['thumbnail_url'];
  kaltura_entry_save($entity);
}

/**
 * Helper function to delete notification.
 *
 * @param $notification
 */
function kaltura_notify_delete($notification) {
  $entity = kaltura_entry_load($notification['entry_id']);
  $entity->kstatus = 3;
  kaltura_entry_save($entity);
}

/**
 * Helper function to block notification.
 *
 * @param $notification
 */
function kaltura_notify_blocked($notification) {
  $entity = kaltura_entry_load($notification['entry_id']);
  $entity->kstatus = 6;
  kaltura_entry_save($entity);
}

/**
 * Helper function to update notification.
 *
 * @param $notification
 */
function kaltura_notify_update($notification) {
  $fields = kaltura_mk_karray($notification);
  $field_names = array(
    'kaltura_entryid',
    'kaltura_tags',
    'kstatus',
    'kaltura_media_type',
    'kaltura_thumbnail_url',
    'kaltura_partner_data',
    'kaltura_width',
    'kaltura_height',
    'kaltura_download_url',
    'kaltura_title',
  );

  $entity = kaltura_entry_load($notification['entry_id']);
  $save = FALSE;

  foreach ($field_names as $key) {
    if ($entity->$key != $fields[$key]) {
      $save = TRUE;
    }

    $entity->$key = $fields[$key];
  }

  if ($save) {
    kaltura_entry_save($entity);
  }
}

/**
 * Adds entry to the kaltura table via notification.
 *
 * @param mixed $type
 * @param mixed $notification_data
 */
function kaltura_notify_entry_add($type, $notification_data) {
  $entry = kaltura_mk_karray($notification_data);
  // We have no way to figure out when the entry was created on the KMC
  // so the create date is when we received the notification.
  $entry['kaltura_created_date'] = REQUEST_TIME;

  $entity = kaltura_entry_create($entry);
  kaltura_entry_save($entity);
}

/**
 * Creates an array of fields to add to kaltura table from the notification.
 *
 * @param mixed $array
 *
 * @return array
 */
function kaltura_mk_karray($array) {
  $ent['kaltura_entryid'] = $array['entry_id'];
  $ent['kaltura_tags'] = $array['tags'];
  $ent['kstatus'] = $array['status'];
  $ent['kaltura_media_type'] = $array['media_type'];
  $ent['kaltura_thumbnail_url'] = $array['thumbnail_url'];
  $ent['kaltura_partner_data'] = $array['partner_data'];
  $ent['kaltura_width'] = !empty($array['width']) ? $array['width'] : 0;
  $ent['kaltura_height'] = !empty($array['height']) ? $array['height'] : 0;
  $ent['kaltura_download_url'] = $array['download_url'];
  $ent['kaltura_title'] = $array['name'];
  $ent['kaltura_puser_id'] = is_numeric($array['puser_id']) ? $array['puser_id'] : 1;
  return $ent;
}
