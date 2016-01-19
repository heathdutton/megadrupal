<?php
/**
 * @file
 * Login include for the Watchdog Notifier module.
 *
 * This file has the breakout of the main function that does the watchdog
 * notifications.
 */

/**
 * Submit handler for the 'scan now' button on the config page.
 */
function watchdog_notifier_run_now_submit() {
  watchdog_notifier_do_that_thing();
  drupal_set_message(t('Watchdog log scan initiated.  Watchlist recipients should be notified of any new messages momentarily.'));
}


/**
 * The 'business logic' for watching the watchdog log.
 */
function watchdog_notifier_do_that_thing() {
  $total_messages_sent = 0;

  // Get the watchlist.
  $watchlist = watchdog_notifier_get_watchlist();

  // If no watchlist values have been defined, just exit.
  if (empty($watchlist)) {
    return;
  }

  $watchdog_notifier_last_run = variable_get('watchdog_notifier_last_run');

  // Start the clock when we're ready to query for log messages.
  $start_time = time();

  foreach ($watchlist as $wl_entry) {
    $query = db_select('watchdog','w')
      ->fields('w')
      ->condition('type', $wl_entry->type, '=')
      ->condition('timestamp', $watchdog_notifier_last_run, '>');
    // The value '-1' corresponds to "All", so only filter if the value is 0 or
    // greater.
    if ($wl_entry->severity >= 0) {
      $query->condition('severity', $wl_entry->severity, '=');
    }
    $result = $query->orderBy('timestamp', 'ASC');
    $result = $query->execute();
    foreach ($result as $res) {
      watchdog_notifier_send_message($res, $wl_entry->email);
      $total_messages_sent += 1;
    }
  }

  $time_delta = time() - $start_time;
  variable_set('watchdog_notifier_last_run', $start_time + $time_delta);

  watchdog(
    'watchdog_notifier',
    'Finished scanning the watchdog log and emailing watchlist recipients. @num message(s) emailed. Runtime was @time seconds',
    array(
      '@num'  => print_r($total_messages_sent, TRUE),
      '@time' => print_r($time_delta, TRUE),
    ),
    WATCHDOG_INFO
  );
}


/**
 * Implements hook_mail().
 */
function watchdog_notifier_mail($key, &$message, $params) {
  switch ($key) {
    case 'information':
      $message['subject'] = $params['subject'];
      $message['body']    = $params['body'];
    break;
    }
}

/**
 * Quick function to return the string equivalent of each of numerical severity
 * level.
 */
function watchdog_notifier_get_severity($notification_level = null) {
  $severity_array = array(
    -1 => 'All',
    0 => 'Emergency',
    1 => 'Alert',
    2 => 'Critical',
    3 => 'Error',
    4 => 'Warning',
    5 => 'Notice',
    6 => 'Informational',
    7 => 'Debug',
  );

  $severity_string = '';
  if ($notification_level == null) {
    $severity_string = 'No severity level has been provided.';
  } else {
    $severity_string = $severity_array[$notification_level];
  }

  return $severity_string;
}


/**
 * Identify the severity and type of watchdog messages to transmit.
 *
 * @return array
 *   The list of type/severity combinations to track.
 */
function watchdog_notifier_get_watchlist() {
  $watchlist = &drupal_static(__FUNCTION__);

  if (!isset($watchlist)) {
    $result = db_select('watchdog_notifier','wn')
      ->fields('wn', array('wn_id', 'email', 'type', 'severity'))
      ->execute();
    foreach ($result as $res) {
      $watchlist[] = $res;
    }
  }

  return $watchlist;
}

/**
 * Transmit a watchdog message.
 *
 * @param array $watchdog
 *   The complete value passed via hook_watchdog().
 * @param string $email
 *   The email address to send the report to.
 *
 * @return array
 *   The results of the drupal_mail() call.
 */
function watchdog_notifier_send_message($watchdog, $email) {
  // If they haven't set the site wide email address, then just use the
  // recipient address as the sender.
  $site_wide_email_address = variable_get('site_mail', $email);
  $current_server = $_SERVER['SERVER_NAME'];

  $email_params = array();

  $result = watchdog_notifier_create_message($watchdog);

  // Notify user as the messages are generated.
  if (is_object($result)) {
    $email_params['subject'] = '[Watchdog Notifier] ~' . $current_server . '~ type: ' . $result->type . ', severity: ' . $result->severity;
    $email_params['body']    = array($result->body_message);

  // TODO: Build a digest of all the messages.
  } else {

  }

  return drupal_mail(
    'watchdog_notifier',
    'information',
    $email,
    language_default(),
    $email_params,
    $site_wide_email_address
  );
}

/**
 * Create the watchdog notification message based on the watchdog database row.
 * This code is basically identical to core_watchdog_format_result() from
 * https://github.com/drush-ops/drush/blob/master/commands/core/watchdog.drush.inc
 *
 * @param array $watchdog_row
 *   The complete value passed via hook_watchdog().
 *
 * @return array
 *   The message to be sent to the recipient(s).
 */
function watchdog_notifier_create_message($watchdog_row) {
  if (is_object($watchdog_row)) {
    $watchdog_row = (array) $watchdog_row;
  }

  $result            = new stdClass();
  $result->type      = $watchdog_row['type'];
  $result->severity  = watchdog_notifier_get_severity($watchdog_row['severity']);
  $result->timestamp = $watchdog_row['timestamp'];
  $result->string    = '';

  // Message.
  $variables = $watchdog_row['variables'];
  if (is_string($variables)) {
    $variables = unserialize($variables);
  }
  if (is_array($variables)) {
    $result->string = strtr($watchdog_row['message'], $variables);
  }

  // Possible empty values.
  if (!empty($watchdog_row['link'])) {
    $result->link = $watchdog_row['link'];
  }
  if (!empty($watchdog_row['referer'])) {
    $result->referer = $watchdog_row['referer'];
  }

  // Username.
  if ($account = user_load($watchdog_row['uid'])) {
    $result->username = $account->name;
  } else {
    $result->username = dt('Anonymous');
  }

  $result->string = truncate_utf8(strip_tags(decode_entities($result->string)), PHP_INT_MAX, FALSE, FALSE);

  $result->body_message = '';
  if (array_key_exists('wid', $watchdog_row)) {
    global $base_url;
    $result->wid          = $watchdog_row['wid'];
    $result->body_message .= '* Watchdog log message url: ' . $base_url . '/admin/reports/event/' . $result->wid . "\n\n";
  } else {
    $result->body_message .= "* No message URL is available because this notification was sent immediately upon logging.\n\n";
  }

  $result->body_message .= '* Watchdog log message timestamp: ' . format_date($result->timestamp) . "\n\n";
  $result->body_message .= "* Watchdog log message contents:\n\n" . $result->string . "\n\n";

  $tmp_result = clone $result;
  unset($tmp_result->body_message);

  $result->body_message .= "* Full message object:\n\n" . print_r($tmp_result, TRUE) . "\n";

  return $result;
}
